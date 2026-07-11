#include "mjpeg_streamer.h"
#include "httplib.h"

// TODO: #include "httplib.h"  (cpp-httplib single header)

MjpegStreamer::MjpegStreamer(int port, int jpeg_quality)
    : port_(port), jpeg_quality_(jpeg_quality), running_(false) {
}

MjpegStreamer::~MjpegStreamer() {
    stop();
}

void MjpegStreamer::start() {
    running_ = true;
    server_thread_ = std::thread(&MjpegStreamer::serve, this);
}

void MjpegStreamer::stop() {
    running_ = false;
    if (server_thread_.joinable()) {
        server_thread_.join();
    }
}

void MjpegStreamer::update_frame(const cv::Mat& frame) {
    std::lock_guard<std::mutex> lock(frame_mutex_);
    current_frame_ = frame.clone();
}

void MjpegStreamer::serve() {
    httplib::Server svr;

    svr.Get("/stream", [this](const httplib::Request&, httplib::Response& res) {
        res.set_content_provider(
            "multipart/x-mixed-replace; boundary=frame",
            [this](size_t offset, httplib::DataSink& sink) {
                while (running_) {
                    // grab the current frame
                    cv::Mat frame;
                    {
                        std::lock_guard<std::mutex> lock(frame_mutex_);
                        if (current_frame_.empty()) {
                            std::this_thread::sleep_for(std::chrono::milliseconds(10));
                            continue;
                        }
                        frame = current_frame_.clone();
                    }

                    // encode to JPEG
                    std::vector<uchar> buffer;
                    cv::imencode(".jpg", frame, buffer,
                        {cv::IMWRITE_JPEG_QUALITY, jpeg_quality_});

                    // write multipart chunk
                    std::string header = "--frame\r\n"
                        "Content-Type: image/jpeg\r\n"
                        "Content-Length: " + std::to_string(buffer.size()) + "\r\n\r\n";

                    sink.write(header.data(), header.size());
                    sink.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());
                    sink.write("\r\n", 2);

                    std::this_thread::sleep_for(std::chrono::milliseconds(33)); // ~30fps
                }
                sink.done();
                return true;
            }
        );
    });

    std::cout << "MJPEG stream at http://localhost:" << port_ << "/stream" << std::endl;
    svr.listen("0.0.0.0", port_);
}

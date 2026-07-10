#include "mjpeg_streamer.h"

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
    // TODO: implement HTTP server with cpp-httplib
    //
    // 1. Create httplib::Server
    // 2. Register GET "/stream" handler:
    //    - Set response content type to "multipart/x-mixed-replace; boundary=frame"
    //    - In a loop while running_:
    //      a. Lock frame_mutex_, copy current_frame_
    //      b. cv::imencode(".jpg", frame, buffer, {cv::IMWRITE_JPEG_QUALITY, jpeg_quality_})
    //      c. Write multipart chunk:
    //         "--frame\r\n"
    //         "Content-Type: image/jpeg\r\n"
    //         "Content-Length: <size>\r\n\r\n"
    //         <jpeg bytes>
    //         "\r\n"
    //      d. Sleep briefly to control frame rate
    // 3. server.listen("0.0.0.0", port_)
}

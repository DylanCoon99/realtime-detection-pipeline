#pragma once

#include <string>
#include <thread>
#include <mutex>
#include <atomic>
#include <opencv2/opencv.hpp>

/*
    MJPEG Streamer - serves annotated frames as an MJPEG stream over HTTP.

    Usage:
        MjpegStreamer streamer(8081);
        streamer.start();

        // in pipeline loop:
        streamer.update_frame(annotated_frame);

        // on shutdown:
        streamer.stop();

    Clients connect to http://<host>:8081/stream to receive the live feed.
    Uses cpp-httplib for the HTTP server (single header, no dependencies).
*/

class MjpegStreamer {
public:
    MjpegStreamer(int port, int jpeg_quality = 80);
    ~MjpegStreamer();

    void start();
    void stop();
    void update_frame(const cv::Mat& frame);

private:
    void serve();

    int port_;
    int jpeg_quality_;
    std::atomic<bool> running_;
    std::thread server_thread_;
    cv::Mat current_frame_;
    std::mutex frame_mutex_;
};

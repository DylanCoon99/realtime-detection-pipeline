#include <chrono>
#include "frame_source.h"


// Source Class method definitions

Source::Source(std::string source_name, int source_id)
    : source_name_(source_name), cap_(source_id), frame_number_(0) {
        
    if (!cap_.isOpened()) {
        throw std::runtime_error("Failed to open video source: " + source_name_);
    }
}



Frame& Source::process() {
    
    // read next image from the video source
    cv::Mat raw_frame;
    cap_ >> raw_frame;
    
    // Capture the current wall-clock time
    std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
    
    // Convert to a numeric timestamp (milliseconds since epoch)
    auto duration = now.time_since_epoch();
    int64_t millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    
    // construct a frame
    struct Frame frame = {
        .image = raw_frame,
        .frame_number = frame_number_,
        .timestamp = millis
    };
    
    // increment the frame number
    ++frame_number_;
    
}

#pragma once

#include "types.h"
#include <opencv2/opencv.hpp>
/*
    - frame_source: Captures frames from a camera or video file.  Outputs a frame (e.g., cv::Mat) to the next
    step in the pipeline.
*/





class Source {
public:
    Source(std::string source_name, int source_id);          // webcam
    Source(std::string source_name, const std::string& path); // video file
    
    Frame process();
    
    std::string name() const {
        return source_name_;
    }
    
private:
    std::string source_name_;
    cv::VideoCapture cap_;
    uint32_t frame_number_;
    
};

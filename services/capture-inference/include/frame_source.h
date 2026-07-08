#pragma once

#include "types.h"
#include <opencv2/opencv.hpp>
/*
    - frame_source: Captures frames from a camera or video file.  Outputs a frame (e.g., cv::Mat) to the next
    step in the pipeline.
*/





class Source {
public:
    // need to find a way to initialize the cv::VideoCapture object (e.g., camera index or video file path)
    Source(std::string source_name, int source_id);
    
    Frame& process();
    
    std::string name() const;
    
private:
    std::string source_name_;
    cv::VideoCapture cap_;
    uint32_t frame_number_;
    
};

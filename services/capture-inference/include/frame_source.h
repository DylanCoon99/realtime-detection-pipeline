#pragma once

#include <opencv2/opencv.hpp>
/*
    - frame_source: Captures frames from a camera or video file.  Outputs a frame (e.g., cv::Mat) to the next
    step in the pipeline.
 
    virtual ~PipelineStep() = default;
    virtual std::any process(const std::any& input) = 0;
    virtual std::string name() const = 0;
*/


class Source {
    
    // need to find a way to initialize the cv::VideoCapture object (e.g., camera index or video file path)
    Source(std::string source_name) : source_name(source_name), cap() {}), cap() {}
    
    Frame process();
    
    std::string name() const;
    
private:
    std::string source_name;
    cv::VideoCapture cap;
    
};

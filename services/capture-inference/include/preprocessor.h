#pragma once

#include <opencv2/opencv.hpp>
/*
    - preprocessor: Prepares the captured frames for inference (e.g., resizing, normalization). Outputs a
    processed frame (e.g., cv::Mat).
 
    virtual ~PipelineStep() = default;
    virtual std::any process(const std::any& input) = 0;
    virtual std::string name() const = 0;
*/


class Preprocessor {
    
    Preprocessor() : {};
    
    ProcessedFrame process();
    
    std::string name() const;
    
private:
    
    
};


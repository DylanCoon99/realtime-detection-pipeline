#pragma once

#include <opencv2/opencv.hpp>
/*
    - preprocessor: Prepares the captured frames for inference (e.g., resizing, normalization). Outputs a
    processed frame (e.g., cv::Mat).
*/


class Preprocessor {
public:
    Preprocessor(int n, int channel, int height, int width, std::string name)
        : n_(n), channel_(channel), height_(height), width_(width), preprocessor_name_(name) { };
    
    TensorData process(const Frame& frame) const;
    
    std::string name() const {
        return preprocessor_name_;
    }
    
private:
    int n_;
    int channel_;
    int height_;
    int width_;
    std::string preprocessor_name_;
    
};


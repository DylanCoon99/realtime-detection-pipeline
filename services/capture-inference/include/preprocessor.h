#pragma once

#include <opencv2/opencv.hpp>
/*
    - preprocessor: Prepares the captured frames for inference (e.g., resizing, normalization). Outputs a
    processed frame (e.g., cv::Mat).
*/


class Preprocessor {
public:
    Preprocessor(int n, int channel, int height, int width)
        : n_(n), channel_(channel), height_(height), width_(width) { };
    
    TensorData& process(const Frame& frame) const;
    
    std::string name() const;
    
private:
    int n_;
    int channel_;
    int height_;
    int width_;
    
};


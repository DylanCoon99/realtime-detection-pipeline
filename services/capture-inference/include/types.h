#pragma once

#include <opencv2/opencv.hpp>
#include <string>
#include <chrono>

// structs for capture inference

struct FrameData {
    uint32_t frame_number;
    int64_t timestamp;
};

struct Frame {
    cv::Mat image;
    FrameData metadata;
};

struct TensorData {
    std::vector<float> data;          // pixel values, CHW layout, normalized [0,1]
    std::vector<int64_t> shape;       // {1, 3, 640, 640}
};

struct DetectionObject {
    cv::Rect bounding_box;
    std::string class_label;
    float confidence_score;
};

struct DetectionResult {
    Frame frame;                              // annotated frame (with boxes drawn)
    std::vector<DetectionObject> detections;
    FrameData metadata;                       // carried from the original frame
};


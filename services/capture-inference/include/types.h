#pragma once

#include <opencv2/opencv.hpp
#include <string>

// structs for capture inference

// Frame
typedef cv::Mat Frame;

// Processed Frame (a frame of the correct size and format for the model; ready for inference)
typedef cv::Mat PreProcessedFrame;

// Detection Object
struct DetectionObject {
    ProcessedFrame frame; // The processed frame from which the detection was made
    cv::Rect bounding_box; // Bounding box of the detected object
    std::string class_label; // Class label of the detected object
    float confidence_score; // Confidence score of the detection
};

// Post Processed Frame
typedef cv::Mat PostProcessedFrame;

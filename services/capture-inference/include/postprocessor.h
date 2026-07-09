#pragma once

#include <string>
#include <opencv2/opencv.hpp>
/*
    - postprocessor: Processes the detection results (e.g., drawing bounding boxes, generating reports).
*/


class Postprocessor {
public:
    Postprocessor(std::string postprocessor_name, float nms_threshold = 0.45f)
        : postprocessor_name_(postprocessor_name), nms_threshold_(nms_threshold) { };
    
    Frame& process(Frame& frame, std::vector<DetectionObject>& detections);
    
    std::string name() const {
        return postprocessor_name_;
    }
    
private:
    std::string postprocessor_name_;
    float nms_threshold_;
};


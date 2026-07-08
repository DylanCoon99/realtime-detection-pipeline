#pragma once

#include <opencv2/opencv.hpp>
/*
     - detector: Performs object detection on the preprocessed frames. Outputs a detection object (e.g.,
    bounding boxes, class labels, confidence scores).
 
    The detector will own the onnx environment (load the model, perform the inference, etc.)
 
*/


class Detector {
public:
    Detector() {};
    
    std::vector<DetectionObject> process(const ProcessedFrame&) const;
    
private:
    
    
};


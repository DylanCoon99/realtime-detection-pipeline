#pragma once

#include <opencv2/opencv.hpp>
/*
     - detector: Performs object detection on the preprocessed frames. Outputs a detection object (e.g.,
    bounding boxes, class labels, confidence scores).
 
    virtual ~PipelineStep() = default;
    virtual std::any process(const std::any& input) = 0;
    virtual std::string name() const = 0;
*/


class Detector {
    
    Detector() : {};
    
    DetectionObject process();
    
    std::string name() const;
    
private:
    
    
};


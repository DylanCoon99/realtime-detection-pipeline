#pragma once

#include <opencv2/opencv.hpp>
/*
    - postprocessor: Processes the detection results (e.g., drawing bounding boxes, generating reports).
*/


class Postprocessor {
public:
    Postprocessor() {};
    
    Frame& process(Frame& frame, std::vector<DetectionObject> detections) const;
    
    std::string name() const;
    
private:
    
    
};


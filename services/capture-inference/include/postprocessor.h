#pragma once

#include <opencv2/opencv.hpp>
/*
    - postprocessor: Processes the detection results (e.g., drawing bounding boxes, generating reports).
 
    virtual ~PipelineStep() = default;
    virtual std::any process(const std::any& input) = 0;
    virtual std::string name() const = 0;
*/


class Postprocessor {
    
    Postprocessor() : {};
    
    PostProcessedFrame process();
    
    std::string name() const;
    
private:
    
    
};


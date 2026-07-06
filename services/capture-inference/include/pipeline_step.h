#pragma once

#include <any>
#include <string>

/*
Each step of the pipeline will inherit from this abstract class
 
 frame_source -> preprocessor -> detector -> postprocessor
 
 frame_source: Captures frames from a camera or video file.
 
 preprocessor: Prepares the captured frames for inference (e.g., resizing, normalization).
 
 detector: Performs object detection on the preprocessed frames.
 
 postprocessor: Processes the detection results (e.g., drawing bounding boxes, generating reports).
 
 
*/


class PipelineStep {
public:
    virtual ~PipelineStep() = default;
    virtual std::any process(const std::any& input) = 0;
    virtual std::string name() const = 0;
};

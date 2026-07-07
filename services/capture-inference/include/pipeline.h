#pragma once

#include <any>
#include <string>


/*
Each step of the pipeline will inherit from this abstract class
 
 frame_source -> preprocessor -> detector -> postprocessor
 
 frame_source: Captures frames from a camera or video file.  Outputs a frame (e.g., cv::Mat) to the next step in the pipeline.
 
 preprocessor: Prepares the captured frames for inference (e.g., resizing, normalization). Outputs a processed frame (e.g., cv::Mat).

 
 detector: Performs object detection on the preprocessed frames. Outputs a detection object (e.g., bounding boxes, class labels, confidence scores).
 
 postprocessor: Processes the detection results (e.g., drawing bounding boxes, generating reports).
 
 
 
*/


class Pipeline {

public:
    Pipeline(const Source& source,
             const Preprocessor& preprocessor,
             const Detector& detector,
             const Postprocessor& postprocessor)
        : source_(source), preprocessor_(preprocessor), detector_(detector), postprocessor_(postprocessor) { }
    
    const PostProcessedFrame execute(const std::any& data) const;
    
private:
    const Source source_;
    const Preprocessor preprocessor_;
    const Detector detector_;
    const Postprocessor postprocessor_;
};

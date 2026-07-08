#include <vector>
#include "pipeline.h"
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


/*

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
 
 
*/




DetectionResult Pipeline::execute() {
      auto frame = source_.process();
      auto tensor = preprocessor_.process(frame);
      auto detections = detector_.process(tensor);
      auto annotated_frame = postprocessor_.process(frame, detections);
      return DetectionResult{annotated_frame, detections, frame.metadata};
  }

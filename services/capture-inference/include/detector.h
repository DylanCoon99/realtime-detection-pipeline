#pragma once

#include <opencv2/opencv.hpp>
#include <onnxruntime_cxx_api.h>
/*
     - detector: Performs object detection on the preprocessed frames. Outputs a detection object (e.g.,
    bounding boxes, class labels, confidence scores).
 
    The detector will own the onnx environment (load the model, perform the inference, etc.)
 
 
*/


class Detector {
public:
    Detector();
    
    std::vector<DetectionObject> process(const TensorData& tensor) const;
    
private:
    Ort::Env env_;
    Ort::SessionOptions session_options_;
    Ort::Session session_;
    const char* model_path_ = "/Users/Dylan/Documents/realtime-detection-pipeline/services/capture-inference/models/yolov8n.onnx";
    
};


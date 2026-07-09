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
    Detector(float confidence_threshold = 0.25f);

    std::vector<DetectionObject> process(TensorData& tensor);
    
private:
    const char* model_path_ = "/Users/Dylan/Documents/realtime-detection-pipeline/services/capture-inference/models/yolov8n.onnx";
    Ort::Env env_;
    Ort::SessionOptions session_options_;
    Ort::Session session_;
    Ort::AllocatorWithDefaultOptions allocator_;
    float confidence_threshold_;

};


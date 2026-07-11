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
    Detector(const std::string& model_path = "", float confidence_threshold = 0.25f);

    std::vector<DetectionObject> process(TensorData& tensor);
    
private:
    std::string model_path_;
    Ort::Env env_;
    Ort::SessionOptions session_options_;
    Ort::Session session_;
    Ort::AllocatorWithDefaultOptions allocator_;
    float confidence_threshold_;

};


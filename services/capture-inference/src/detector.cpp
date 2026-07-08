#include "types.h"
#include "detector.h"
/*
 
    // Onnx test
    // 1. Initialize the ONNX Runtime Environment
    Ort::Env env(ORT_LOGGING_LEVEL_WARNING, "ONNX_Inference");

    // 2. Configure Session Options
    Ort::SessionOptions session_options;
    session_options.SetIntraOpNumThreads(1);
    session_options.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_ALL);

    // 3. Load the Model and Create a Session
    const char* model_path = "/Users/Dylan/Documents/realtime-detection-pipeline/services/capture-inference/models/yolov8n.onnx";

    Ort::Session session(env, model_path, session_options);
 
*/


static Ort::SessionOptions make_session_options() {
    Ort::SessionOptions opts;
    opts.SetIntraOpNumThreads(1);
    opts.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_ALL);
    return opts;
}

Detector::Detector()
    : env_(ORT_LOGGING_LEVEL_WARNING, "ONNX_Inference"),
      session_options_(make_session_options()),
      session_(env_, model_path_, session_options_) {
}


std::vector<DetectionObject> Detector::process(const TensorData& tensor) const {

    

}

#include "types.h"
#include "detector.h"
#include "coco_classes.h"

/*

Step 4: Wrap the data as an ONNX tensor
                                                                                                        
    auto memory_info = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);
    Ort::Value input_tensor = Ort::Value::CreateTensor<float>(
       memory_info,
       input_tensor_values.data(),
       input_tensor_values.size(),
       input_shape.data(),
       input_shape.size()
    );
    ONNX Runtime doesn't work with raw vectors — it needs an Ort::Value tensor object. MemoryInfo tells it "this data lives on the
    CPU." CreateTensor wraps your existing float buffer as a tensor without copying it — it just points to your vector's memory with
    the shape metadata attached.
                           
Step 5: Run inference
                                                                                                            
    const char* input_names[] = {input_name.get()};
    const char* output_names[] = {output_name.get()};
                                                                                                            
    std::vector<Ort::Value> output_tensors = session.Run(
       Ort::RunOptions{nullptr},
       input_names,
       &input_tensor, 1,
       output_names, 1
    );
    This is the actual model execution. You pass:
    - input_names — which inputs you're providing (by name)
    - &input_tensor, 1 — the tensor data and count (1 input)
    - output_names, 1 — which outputs you want back (1 output)
                                                                                                                                    
    The model processes the input through its neural network layers and returns the results. For YOLOv8n, this takes a few
    milliseconds on desktop, maybe 100-200ms on a Pi.
                                                                                                                                
Step 6: Read the output
                                                                                                                
    auto output_type_info = output_tensors[0].GetTensorTypeAndShapeInfo();
    auto output_shape = output_type_info.GetShape();
    float* float_array = output_tensors[0].GetTensorMutableData<float>();
    The output is another Ort::Value tensor. For YOLOv8n the shape is {1, 84, 8400}:
    - 1 — batch size
    - 84 — 4 bbox coordinates (x_center, y_center, width, height) + 80 COCO class confidence scores
    - 8400 — number of candidate detections
                                                                                                                                    
    GetTensorMutableData<float>() gives you a raw float* pointer to the output data. Each of the 8400 candidates has 84 values. Your
    postprocessor's job will be to filter these by confidence threshold and apply NMS to get the final detections.
     
*/


static Ort::SessionOptions make_session_options() {
    Ort::SessionOptions opts;
    opts.SetIntraOpNumThreads(1);
    opts.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_ALL);
    return opts;
}


Detector::Detector(float confidence_threshold)
    : model_path_("/Users/Dylan/Documents/realtime-detection-pipeline/services/capture-inference/models/yolov8n.onnx"),
      env_(ORT_LOGGING_LEVEL_WARNING, "ONNX_Inference"),
      session_options_(make_session_options()),
      session_(env_, model_path_, session_options_),
      allocator_(),
      confidence_threshold_(confidence_threshold) {
}


std::vector<DetectionObject> Detector::process(TensorData& tensor) {
    
    std::vector<DetectionObject> result;
    
    // input/output names
    auto input_name = session_.GetInputNameAllocated(0, allocator_);
    auto output_name = session_.GetOutputNameAllocated(0, allocator_);
    
    // Step 4: Wrap the data as an ONNX tensor
    auto memory_info = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);
    Ort::Value input_tensor = Ort::Value::CreateTensor<float>(
        memory_info,
        tensor.data.data(),
        tensor.data.size(),
        tensor.shape.data(),
        tensor.shape.size()
    );
    
    // Step 5: Run inference
    const char* input_names[] = {input_name.get()};
    const char* output_names[] = {output_name.get()};
                                                                                                            
    std::vector<Ort::Value> output_tensors = session_.Run(
       Ort::RunOptions{nullptr},
       input_names,
       &input_tensor, 1,
       output_names, 1
    );
    
    
    // Step 6: Read the output
    auto output_type_info = output_tensors[0].GetTensorTypeAndShapeInfo();
    auto output_shape = output_type_info.GetShape();
    float* float_array = output_tensors[0].GetTensorMutableData<float>();
    
    /*
        The output shape is {1, 84, 8400}, which means the float* is a flat array of 1 * 84 * 8400 = 705,600 floats.
                                                                                                                                        
        It's laid out as 84 rows of 8400 values each:
                                                                                                            
        Row 0:  x_center for all 8400 candidates
        Row 1:  y_center for all 8400 candidates
        Row 2:  width    for all 8400 candidates
        Row 3:  height   for all 8400 candidates
        Row 4:  class_0 confidence (person) for all 8400 candidates
        Row 5:  class_1 confidence (bicycle) for all 8400 candidates
        ...
        Row 83: class_79 confidence (toothbrush) for all 8400 candidates
                                                                                                            
        So to access candidate i (where i is 0 to 8399):
                                                                                                            
        x_center   = data[0 * 8400 + i]
        y_center   = data[1 * 8400 + i]
        width      = data[2 * 8400 + i]
        height     = data[3 * 8400 + i]
        class_0    = data[4 * 8400 + i]
        class_1    = data[5 * 8400 + i]
        ...
        class_79   = data[83 * 8400 + i]
     For each candidate, you'd find which class has the highest confidence score (the max across rows 4-83 at column i), and if that
       score exceeds your threshold (e.g., 0.5), it's a detection. The bbox coordinates are in pixel space relative to the 640x640 input
        image.

    */
    
    int num_candidates = static_cast<int>(output_shape[2]); // not sure how to find this programmatically
    int num_classes = static_cast<int>(output_shape[1]) - 4;
    
    // for each candidate
    
    for (int i = 0; i < num_candidates; ++i) {
        float confidence;
        int class_with_max_confidence;
        // determine which class has the highest confidence score
        float max_confidence = 0.0;
        for (int class_n = 0; class_n < num_classes; ++class_n) {
            confidence = float_array[(class_n + 4) * num_candidates + i];
            if (confidence > max_confidence) {
                max_confidence = confidence;
                class_with_max_confidence = class_n;
            }
        }
        // if the score exceeds threshold
        if (max_confidence > confidence_threshold_) {
            // -> it's a detection
            // create a bounding box
            float x_center = float_array[i];
            float y_center = float_array[1 * num_candidates + i];
            float width = float_array[2 * num_candidates + i];
            float height = float_array[3 * num_candidates + i];
            
            
            cv::Rect bbox(x_center - (width / 2), y_center - (height / 2), width, height);
            // class label: class_n is the class number but I am not sure how to find the string label
            std::string class_label = COCO_CLASSES.at(class_with_max_confidence);
            struct DetectionObject detection = {
                bbox,
                class_label,
                max_confidence
            };
            
            result.push_back(detection);
        }
       
    }
    
    return result;
        
}

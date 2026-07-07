#include <iostream>
#include <opencv2/opencv.hpp>
#include <onnxruntime_cxx_api.h>


int main() {
    /*
    // 1. Load an image from disk (Reads in BGR format by default)
    cv::Mat image = cv::imread("/Users/Dylan/Pictures/Saved Pictures/don_frye.jpg", cv::IMREAD_COLOR);

    // 2. Error handling: Check if the image loaded successfully
    if (image.empty()) {
        std::cerr << "Error: Could not open or find the image!" << std::endl;
        return -1;
    }

    
    // 3. Process the image: Convert the color space to Grayscale
    cv::Mat gray_image;
    cv::cvtColor(image, gray_image, cv::COLOR_BGR2GRAY);

    
    // 4. Create UI windows and display the images
    cv::namedWindow("Original Image", cv::WINDOW_AUTOSIZE);
    cv::namedWindow("Processed Image", cv::WINDOW_AUTOSIZE);

    
    cv::imshow("Original Image", image);
    cv::imshow("Processed Image", gray_image);

    
    // 5. Keep windows open indefinitely until any key is pressed
    cv::waitKey(0);
    */
    
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
    
    // 4. Query the model for input/output names and shape
    Ort::AllocatorWithDefaultOptions allocator;

    auto input_name = session.GetInputNameAllocated(0, allocator);
    auto output_name = session.GetOutputNameAllocated(0, allocator);
    std::cout << "Input name: " << input_name.get() << std::endl;
    std::cout << "Output name: " << output_name.get() << std::endl;

    auto input_type_info = session.GetInputTypeInfo(0);
    auto tensor_info = input_type_info.GetTensorTypeAndShapeInfo();
    auto input_shape = tensor_info.GetShape();

    std::cout << "Input shape: ";
    for (auto dim : input_shape) std::cout << dim << " ";
    std::cout << std::endl;

    // 5. Create mock input data matching YOLOv8n shape: (1, 3, 640, 640)
    size_t input_tensor_size = 1;
    for (auto dim : input_shape) input_tensor_size *= dim;

    std::vector<float> input_tensor_values(input_tensor_size, 0.5f);

    auto memory_info = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);
    Ort::Value input_tensor = Ort::Value::CreateTensor<float>(
        memory_info,
        input_tensor_values.data(),
        input_tensor_values.size(),
        input_shape.data(),
        input_shape.size()
    );

    // 6. Execute Inference
    const char* input_names[] = {input_name.get()};
    const char* output_names[] = {output_name.get()};

    std::vector<Ort::Value> output_tensors = session.Run(
        Ort::RunOptions{nullptr},
        input_names,
        &input_tensor, 1,
        output_names, 1
    );

    // 7. Access the Output Tensor
    auto output_type_info = output_tensors[0].GetTensorTypeAndShapeInfo();
    auto output_shape = output_type_info.GetShape();

    std::cout << "Output shape: ";
    for (auto dim : output_shape) std::cout << dim << " ";
    std::cout << std::endl;

    float* float_array = output_tensors[0].GetTensorMutableData<float>();
    std::cout << "First output value: " << float_array[0] << std::endl;
    
    
    return 0;
}

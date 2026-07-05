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
    const char* model_path = "model.onnx";

    Ort::Session session(env, model_path, session_options);

    // 4. Set up Mock Data (Example: 1 input image/vector of size 4)
    std::vector<float> input_tensor_values = {1.0f, 2.0f, 3.0f, 4.0f};
    std::vector<int64_t> input_shape = {1, 4}; // Batch size = 1, Features = 4

    // 5. Wrap your native buffer into an Ort::Value (Tensor)
    auto memory_info = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);
    Ort::Value input_tensor = Ort::Value::CreateTensor<float>(
        memory_info,
        input_tensor_values.data(),
        input_tensor_values.size(),
        input_shape.data(),
        input_shape.size()
    );

    // 6. Define IO Names (Match your model's exact metadata)
    const char* input_names[] = {"input_node_name"};
    const char* output_names[] = {"output_node_name"};

    // 7. Execute Inference
    std::vector<Ort::Value> output_tensors = session.Run(
        Ort::RunOptions{nullptr},
        input_names,
        &input_tensor, 1, // Number of inputs
        output_names, 1   // Number of outputs
    );

    // 8. Access the Output Tensor Values
    float* float_array = output_tensors[0].GetTensorMutableData<float>();
    std::cout << "Top output prediction: " << float_array[0] << std::endl;
    
    
    return 0;
}

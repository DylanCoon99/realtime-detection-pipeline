#include <iostream>
#include <opencv2/opencv.hpp>
#include <onnxruntime_cxx_api.h>

int main() {
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
    return 0;
}

#include "types.h"
#include "preprocessor.h"
/*

 class Preprocessor {
 public:
     Preprocessor(int n, int channel, int height, int width)
         : n_(n), channel_(channel), height_(height), width_(width) { };
     
     TensorData& process(const Frame& frame) const;
     
     std::string name() const;
     
 private:
     int n_;
     int channel_;
     int height_;
     int width_;
     
 };
 
 
 
 struct TensorData {
     std::vector<float> data;          // pixel values, CHW layout, normalized [0,1]
     std::vector<int64_t> shape;       // {1, 3, 640, 640}
 };


 
*/


TensorData Preprocessor::process(const Frame& frame) const {
    
    /*
    
     cv::Mat cv::dnn::blobFromImage(
         cv::InputArray image,
         double scalefactor = 1.0,
         const cv::Size& size = cv::Size(),
         const cv::Scalar& mean = cv::Scalar(),
         bool swapRB = false,
         bool crop = false,
         int ddepth = CV_32F
     );

     
    */
    
    // Resize, normalize (0-255 -> 0.0-1.0), BGR->RGB, and convert to NCHW layout
    cv::Mat blob = cv::dnn::blobFromImage(
        frame.image,
        1.0 / 255.0,
        cv::Size(width_, height_),
        cv::Scalar(0),
        true,    // swapRB: BGR -> RGB
        false,
        CV_32F
    );

    // blob is a 4D Mat (1x3x640x640), already contiguous NCHW float data
    float* ptr = blob.ptr<float>(0);
    size_t total = blob.total();

    return TensorData{
        std::vector<float>(ptr, ptr + total),
        {n_, channel_, height_, width_}
    };
    
}

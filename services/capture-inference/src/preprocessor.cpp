#include "types.h"
#include "preprocessor.h"


TensorData Preprocessor::process(const Frame& frame) const {
    
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

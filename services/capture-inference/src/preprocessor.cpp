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


TensorData& Preprocessor::process(const Frame& frame) const {
    
    
    
    
    struct TensorData& tensor = {
        .data = ;
        .shape = {n_, channel_, height_, width_};
    };
    
    
    return tensor;
    
}

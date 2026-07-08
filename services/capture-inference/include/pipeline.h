#pragma once

#include <any>
#include <string>
#include "types.h"
#include "frame_source.h"
#include "preprocessor.h"
#include "detector.h"
#include "postprocessor.h"


class Pipeline {
public:
    Pipeline(Source& source,
             Preprocessor& preprocessor,
             Detector& detector,
             Postprocessor& postprocessor)
        : source_(source), preprocessor_(preprocessor), detector_(detector),
          postprocessor_(postprocessor) { }
    
    DetectionResult execute();
    
private:
    Source& source_;
    Preprocessor& preprocessor_;
    Detector& detector_;
    Postprocessor& postprocessor_;
};

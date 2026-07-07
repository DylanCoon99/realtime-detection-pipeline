#include <vector>
#include "pipeline_step.h"
#include <any>
#include <string>


void Pipeline::add_step(const PipelineStep& step) {
    pipeline.push_back(step);
}


const std::any Pipeline::execute(const std::any& data) const {
    std::any result = data;
    for (const auto& step : pipeline) {
        result = step.process(result);
    }
    return result;
}



#include <iostream>
#include <opencv2/opencv.hpp>
#include <onnxruntime_cxx_api.h>
#include "pipeline.h"
#include "mjpeg_streamer.h"


int main() {
    
    // Pipeline steps
    Source source("webcam", 0);
    Preprocessor preprocessor(1, 3, 640, 640, "preprocessor");
    Detector detector("", 0.5f);
    Postprocessor postprocessor("postprocessor");
    
    // Pipeline
    Pipeline pipeline(source, preprocessor, detector, postprocessor);
    bool running = true;
    
    // MJPEG Streamer
    MjpegStreamer streamer(8081, 100);
    streamer.start();

    std::cout << "Pipeline running. Press Ctrl+C to stop." << std::endl;

    while (running) {
        DetectionResult result = pipeline.execute();

        if (result.frame.image.empty()) break;

        streamer.update_frame(result.frame.image);
        // publisher.publish(result.detections);  // kafka later
    }

    streamer.stop();
    return 0;
}

#include <iostream>
#include <cstdlib>
#include <opencv2/opencv.hpp>
#include <onnxruntime_cxx_api.h>
#include "pipeline.h"
#include "mjpeg_streamer.h"
#include "kafka_publisher.h"


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
    MjpegStreamer streamer(8081, 80);
    streamer.start();

    // Kafka Publisher
    std::cerr << "[main] Creating Kafka publisher..." << std::endl;
    const char* kafka_broker = std::getenv("KAFKA_BROKER");
    KafkaPublisher publisher(kafka_broker ? kafka_broker : "localhost:29092", "detections");
    std::cerr << "[main] Kafka publisher created." << std::endl;

    std::cerr << "[main] Pipeline running. Press Ctrl+C to stop." << std::endl;

    while (running) {
        DetectionResult result = pipeline.execute();

        if (result.frame.image.empty()) break;

        streamer.update_frame(result.frame.image);
        std::cerr << "[main] Frame " << result.metadata.frame_number
                  << ", detections: " << result.detections.size() << std::endl;
        publisher.publish(result.detections, result.metadata);
    }

    streamer.stop();
    return 0;
}

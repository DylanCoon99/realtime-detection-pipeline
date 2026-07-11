#include <iostream>
#include "kafka_consumer.h"

int main() {
    KafkaConsumer consumer("localhost:29092", "detections", "stream-processor");

    std::cerr << "[main] Consuming from detections topic..." << std::endl;

    consumer.start([](const std::string& message) {
        if (!message.empty()) {
            std::cout << "[main] Received: " << message << std::endl;
        }
    });

    return 0;
}

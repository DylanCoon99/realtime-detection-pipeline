#include <iostream>
#include "kafka_consumer.h"
#include "aggregator.h"
#include "redis_writer.h"

int main() {
    KafkaConsumer consumer("localhost:29092", "detections", "stream-processor");
    Aggregator aggregator;
    RedisWriter writer("localhost", 6379);

    std::cerr << "[main] Stream processor running..." << std::endl;

    consumer.start([&](const std::string& message) {
        if (!message.empty()) {
            aggregator.process(message);
            writer.write(aggregator.state());
        }
    });

    return 0;
}

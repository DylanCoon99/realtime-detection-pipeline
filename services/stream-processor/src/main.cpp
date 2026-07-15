#include <iostream>
#include <cstdlib>
#include "kafka_consumer.h"
#include "aggregator.h"
#include "redis_writer.h"

int main() {
    const char* kafka_broker = std::getenv("KAFKA_BROKER");
    const char* redis_host = std::getenv("REDIS_HOST");
    int redis_port = std::getenv("REDIS_PORT") ? std::atoi(std::getenv("REDIS_PORT")) : 6379;

    KafkaConsumer consumer(
        kafka_broker ? kafka_broker : "localhost:29092",
        "detections",
        "stream-processor"
    );
    Aggregator aggregator;
    RedisWriter writer(
        redis_host ? redis_host : "localhost",
        redis_port
    );

    std::cerr << "[main] Stream processor running..." << std::endl;

    consumer.start([&](const std::string& message) {
        if (!message.empty()) {
            aggregator.process(message);
            writer.write(aggregator.state());
        }
    });

    return 0;
}

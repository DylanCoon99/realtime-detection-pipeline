#pragma once

#include <string>
#include <functional>
#include <librdkafka/rdkafkacpp.h>

/*
    KafkaConsumer — consumes detection events from a Kafka topic.

    Usage:
        KafkaConsumer consumer("localhost:9092", "detections", "stream-processor");
        consumer.start([](const std::string& message) {
            // handle message
        });
        // ...
        consumer.stop();
*/

class KafkaConsumer {
public:
    KafkaConsumer(const std::string& broker, const std::string& topic, const std::string& group_id);
    ~KafkaConsumer();

    // Begin consuming. Calls the callback for each message received.
    void start(std::function<void(const std::string&)> callback);

    // Stop consuming.
    void stop();

    // Poll for a single message. Returns empty string if no message available.
    std::string poll(int timeout_ms = 100);

    bool is_running() const;

private:
    std::string topic_;
    std::string group_id_;
    RdKafka::KafkaConsumer* consumer_;
    bool running_;
};

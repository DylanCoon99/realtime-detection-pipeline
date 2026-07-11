#pragma once

#include <string>
#include <librdkafka/rdkafkacpp.h>
#include "types.h"

/*
    KafkaPublisher — publishes detection events to a Kafka topic as JSON.

    Usage:
        KafkaPublisher publisher("localhost:9092", "detections");
        publisher.publish(result.detections, result.metadata);
*/

class KafkaPublisher {
public:
    KafkaPublisher(const std::string& broker, const std::string& topic);
    ~KafkaPublisher();

    void publish(const std::vector<DetectionObject>& detections, const FrameData& metadata);

private:
    std::string serialize(const std::vector<DetectionObject>& detections, const FrameData& metadata);

    std::string topic_name_;
    RdKafka::Producer* producer_;
};

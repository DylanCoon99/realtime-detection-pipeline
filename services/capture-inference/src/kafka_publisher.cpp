#include "kafka_publisher.h"
#include "json.hpp"
#include <iostream>

KafkaPublisher::KafkaPublisher(const std::string& broker, const std::string& topic)
    : topic_name_(topic), producer_(nullptr) {

    std::string errstr;
    RdKafka::Conf* conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
    conf->set("bootstrap.servers", broker, errstr);

    producer_ = RdKafka::Producer::create(conf, errstr);
    if (!producer_) {
        throw std::runtime_error("Failed to create Kafka producer: " + errstr);
    }
    std::cout << "[KafkaPublisher] Connected to broker: " << broker << ", topic: " << topic << std::endl;

    delete conf;
}

KafkaPublisher::~KafkaPublisher() {
    if (producer_) {
        producer_->flush(1000);
        delete producer_;
    }
}

void KafkaPublisher::publish(const std::vector<DetectionObject>& detections, const FrameData& metadata) {
    std::string payload = serialize(detections, metadata);

    std::cout << "[KafkaPublisher] Publishing frame " << metadata.frame_number
              << " (" << detections.size() << " detections, "
              << payload.size() << " bytes)" << std::endl;

    RdKafka::ErrorCode err = producer_->produce(
        topic_name_,
        RdKafka::Topic::PARTITION_UA,
        RdKafka::Producer::RK_MSG_COPY,
        const_cast<char*>(payload.c_str()),
        payload.size(),
        nullptr, 0,    // key
        0,             // timestamp (auto)
        nullptr        // headers
    );

    if (err != RdKafka::ERR_NO_ERROR) {
        std::cerr << "[KafkaPublisher] Produce FAILED: " << RdKafka::err2str(err) << std::endl;
    } else {
        std::cout << "[KafkaPublisher] Produce OK" << std::endl;
    }

    producer_->poll(0);

    // check outgoing queue size
    int queue_len = producer_->outq_len();
    if (queue_len > 0) {
        std::cout << "[KafkaPublisher] Outgoing queue: " << queue_len << " messages" << std::endl;
    }
}

std::string KafkaPublisher::serialize(const std::vector<DetectionObject>& detections, const FrameData& metadata) {
    using json = nlohmann::json;

    json j;
    j["frame_number"] = metadata.frame_number;
    j["timestamp"] = metadata.timestamp;
    j["detections"] = json::array();

    for (const auto& d : detections) {
        j["detections"].push_back({
            {"class", d.class_label},
            {"confidence", d.confidence_score},
            {"bbox", {
                {"x", d.bounding_box.x},
                {"y", d.bounding_box.y},
                {"w", d.bounding_box.width},
                {"h", d.bounding_box.height}
            }}
        });
    }

    return j.dump();
}

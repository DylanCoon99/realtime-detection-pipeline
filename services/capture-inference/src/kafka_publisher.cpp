#include "kafka_publisher.h"
#include <iostream>
#include <sstream>

KafkaPublisher::KafkaPublisher(const std::string& broker, const std::string& topic)
    : topic_name_(topic), producer_(nullptr) {

    std::string errstr;
    RdKafka::Conf* conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
    conf->set("bootstrap.servers", broker, errstr);

    producer_ = RdKafka::Producer::create(conf, errstr);
    if (!producer_) {
        throw std::runtime_error("Failed to create Kafka producer: " + errstr);
    }

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
        std::cerr << "Kafka produce failed: " << RdKafka::err2str(err) << std::endl;
    }

    producer_->poll(0);
}

std::string KafkaPublisher::serialize(const std::vector<DetectionObject>& detections, const FrameData& metadata) {
    // TODO: consider using a JSON library (nlohmann/json) for robustness.
    //       Manual serialization works for now since the schema is simple.

    std::ostringstream json;
    json << "{";
    json << "\"frame_number\":" << metadata.frame_number << ",";
    json << "\"timestamp\":" << metadata.timestamp << ",";
    json << "\"detections\":[";

    for (size_t i = 0; i < detections.size(); i++) {
        const auto& d = detections[i];
        json << "{";
        json << "\"class\":\"" << d.class_label << "\",";
        json << "\"confidence\":" << d.confidence_score << ",";
        json << "\"bbox\":{";
        json << "\"x\":" << d.bounding_box.x << ",";
        json << "\"y\":" << d.bounding_box.y << ",";
        json << "\"w\":" << d.bounding_box.width << ",";
        json << "\"h\":" << d.bounding_box.height;
        json << "}";
        json << "}";
        if (i < detections.size() - 1) json << ",";
    }

    json << "]}";
    return json.str();
}

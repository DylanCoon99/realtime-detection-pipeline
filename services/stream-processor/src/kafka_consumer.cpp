#include "kafka_consumer.h"
#include <iostream>

KafkaConsumer::KafkaConsumer(const std::string& broker, const std::string& topic, const std::string& group_id)
    : topic_(topic), group_id_(group_id), consumer_(nullptr), running_(false) {
    // TODO: create RdKafka::Conf, set broker + group.id, create KafkaConsumer, subscribe to topic
}

KafkaConsumer::~KafkaConsumer() {
    // TODO: stop consuming, close consumer, delete consumer
}

void KafkaConsumer::start(std::function<void(const std::string&)> callback) {
    // TODO: set running_ = true, loop calling poll(), pass result to callback
}

void KafkaConsumer::stop() {
    // TODO: set running_ = false
}

std::string KafkaConsumer::poll(int timeout_ms) {
    // TODO: call consumer_->consume(timeout_ms), extract payload string, return it
    return "";
}

bool KafkaConsumer::is_running() const {
    return running_;
}

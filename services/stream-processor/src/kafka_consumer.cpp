#include "kafka_consumer.h"
#include <iostream>

KafkaConsumer::KafkaConsumer(const std::string& broker, const std::string& topic, const std::string& group_id)
    : topic_(topic), group_id_(group_id), consumer_(nullptr), running_(false) {
    // TODO: create RdKafka::Conf, set broker + group.id, create KafkaConsumer, subscribe to topic
        
    std::string errstr;
    RdKafka::Conf* conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
    conf->set("bootstrap.servers", broker, errstr);

    if (conf->set("group.id", group_id, errstr) != RdKafka::Conf::CONF_OK) {
        delete conf;
        throw std::runtime_error("Kafka config error: " + errstr);
    }

    // 3. Create the KafkaConsumer instance
    consumer_ = RdKafka::KafkaConsumer::create(conf, errstr);
    if (!consumer_) {
        delete conf;
        throw std::runtime_error("Failed to create Kafka consumer: " + errstr);
    }
    delete conf;

    // Subscribe to topic
    RdKafka::ErrorCode err = consumer_->subscribe({topic_});
    if (err != RdKafka::ERR_NO_ERROR) {
        throw std::runtime_error("Failed to subscribe to topic: " + RdKafka::err2str(err));
    }

    std::cout << "[KafkaConsumer] Subscribed to " << topic_ << " on " << broker << std::endl;
}

KafkaConsumer::~KafkaConsumer() {
    // TODO: stop consuming, close consumer, delete consumer
    stop();
    consumer_->close();
    delete consumer_;
}

void KafkaConsumer::start(std::function<void(const std::string&)> callback) {
    // TODO: set running_ = true, loop calling poll(), pass result to callback
    running_ = true;
    while(running_) {
        callback(poll());
    }
}

void KafkaConsumer::stop() {
    // TODO: set running_ = false
    running_ = false;
}

std::string KafkaConsumer::poll(int timeout_ms) {
    RdKafka::Message* message = consumer_->consume(timeout_ms);
    std::string result;

    switch (message->err()) {
        case RdKafka::ERR_NO_ERROR:
            // valid message — extract payload as string
            result = std::string(static_cast<const char*>(message->payload()), message->len());
            break;
        case RdKafka::ERR__TIMED_OUT:
            // no message available within timeout — not an error
            break;
        case RdKafka::ERR__PARTITION_EOF:
            // reached end of partition — not an error
            break;
        default:
            std::cerr << "[KafkaConsumer] Poll error: " << message->errstr() << std::endl;
            break;
    }

    delete message;
    return result;
}

bool KafkaConsumer::is_running() const {
    return running_;
}

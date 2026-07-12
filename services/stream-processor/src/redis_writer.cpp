#include "redis_writer.h"
#include "json.hpp"
#include <iostream>

RedisWriter::RedisWriter(const std::string& host, int port)
    : ctx_(nullptr) {
    // TODO: connect to Redis with redisConnect(host, port), check for errors
}

RedisWriter::~RedisWriter() {
    // TODO: free context with redisFree(ctx_)
}

void RedisWriter::write(const AggregatedState& state) {
    // TODO: call individual write methods
}

void RedisWriter::write_class_counts(const std::unordered_map<std::string, int>& counts) {
    // TODO: HSET detections:class_counts for each class -> count
}

void RedisWriter::write_recent_events(const std::vector<DetectionEvent>& events) {
    // TODO: DEL + RPUSH detections:recent with JSON-serialized events
}

void RedisWriter::write_timeline(const std::unordered_map<std::string, std::deque<std::pair<int64_t, int>>>& timeline) {
    // TODO: HSET detections:timeline for each class -> JSON array of (bucket, count) pairs
}

bool RedisWriter::is_connected() const {
    return ctx_ != nullptr && ctx_->err == 0;
}

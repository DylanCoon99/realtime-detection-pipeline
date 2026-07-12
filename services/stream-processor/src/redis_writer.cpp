#include "redis_writer.h"
#include "json.hpp"
#include <iostream>

RedisWriter::RedisWriter(const std::string& host, int port)
    : ctx_(nullptr) {
    // TODO: connect to Redis with redisConnect(host, port), check for errors
    ctx_ = redisConnect(host.c_str(), port);
    if (ctx_ == nullptr) {
        throw std::runtime_error("Redis connection error: can't allocate redis context");
    }
    if (ctx_->err) {
        std::string err = ctx_->errstr;
        redisFree(ctx_);
        ctx_ = nullptr;
        throw std::runtime_error("Redis connection error: " + err);
    }
    std::cout << "[RedisWriter] Connected to " << host << ":" << port << std::endl;
}

RedisWriter::~RedisWriter() {
    // TODO: free context with redisFree(ctx_)
    if (ctx_) redisFree(ctx_);
}

void RedisWriter::write(const AggregatedState& state) {
    // TODO: call individual write methods
    write_class_counts(state.class_counts);
    write_recent_events(state.recent_events);
    write_timeline(state.timeline);
}

void RedisWriter::write_class_counts(const std::unordered_map<std::string, int>& counts) {
    // TODO: HSET detections:class_counts for each class -> count
    for (auto& [class_label, count] : counts) {
        redisCommand(ctx_, ("HSET detections:class_counts " + class_label + " " + std::to_string(count)).c_str());
    }
}

void RedisWriter::write_recent_events(const std::vector<DetectionEvent>& events) {
    // TODO: DEL + RPUSH detections:recent with JSON-serialized events
    
    nlohmann::json json_events = nlohmann::json::array();
    
    for (const auto& e : events) {
        nlohmann::json detections = nlohmann::json::array();
        
        for (const auto& d : e.detections) {
            detections.push_back({
                {"class", d.class_label},
                {"confidence", d.confidence},
                {"bbox", {
                    {"x", d.bbox.x},
                    {"y", d.bbox.y},
                    {"w", d.bbox.w},
                    {"h", d.bbox.h}
                }}
            });
        }
        
        
        json_events.push_back({
            {"frame_number", e.frame_number},
            {"timestamp", e.timestamp},
            {"detections", detections}
        });
    }
    
    std::string json_events_string = json_events.dump();
    
    // DEL + RPUSH detections:recent
    redisCommand(ctx_, "DEL detections:recent");
    for (const auto& e : json_events) {
        std::string event_str = e.dump();
        redisCommand(ctx_, "RPUSH detections:recent %s", event_str.c_str());
    }
    
}

void RedisWriter::write_timeline(const std::unordered_map<std::string, std::deque<std::pair<int64_t, int>>>& timeline) {
    for (const auto& [label, deque] : timeline) {
        nlohmann::json buckets = nlohmann::json::array();
        for (const auto& [minute, count] : deque) {
            buckets.push_back({{"minute", minute}, {"count", count}});
        }
        std::string json_str = buckets.dump();
        redisCommand(ctx_, "HSET detections:timeline %s %s", label.c_str(), json_str.c_str());
    }
}

bool RedisWriter::is_connected() const {
    return ctx_ != nullptr && ctx_->err == 0;
}

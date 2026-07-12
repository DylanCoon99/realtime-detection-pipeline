#pragma once

#include <string>
#include <deque>
#include <hiredis/hiredis.h>
#include "types.h"

/*
    RedisWriter — writes aggregated detection state to Redis.

    Keys:
    - "detections:class_counts"   — hash of class -> count
    - "detections:recent"         — list of recent detection events (JSON)
    - "detections:timeline"       — hash of class -> JSON array of (bucket, count) pairs
*/

class RedisWriter {
public:
    RedisWriter(const std::string& host, int port = 6379);
    ~RedisWriter();

    // Write the full aggregated state to Redis
    void write(const AggregatedState& state);

    // Write individual components
    void write_class_counts(const std::unordered_map<std::string, int>& counts);
    void write_recent_events(const std::vector<DetectionEvent>& events);
    void write_timeline(const std::unordered_map<std::string, std::deque<std::pair<int64_t, int>>>& timeline);

    bool is_connected() const;

private:
    redisContext* ctx_;
};

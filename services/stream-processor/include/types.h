#pragma once

#include <string>
#include <vector>
#include <deque>
#include <cstdint>
#include <unordered_map>
#include <utility>

struct BoundingBox {
    int x;
    int y;
    int w;
    int h;
};

struct Detection {
    std::string class_label;
    float confidence;
    BoundingBox bbox;
};

struct DetectionEvent {
    uint32_t frame_number;
    int64_t timestamp;
    std::vector<Detection> detections;
};

// Aggregated state that gets written to Redis
struct AggregatedState {
    std::unordered_map<std::string, int> class_counts;         // total detections per class
    std::vector<DetectionEvent> recent_events;                  // last N events for the feed
    std::unordered_map<std::string, std::deque<std::pair<int64_t, int>>> timeline; // class -> deque of (minute_bucket, count)
};

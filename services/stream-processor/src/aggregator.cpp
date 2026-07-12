#include "aggregator.h"
#include "json.hpp"

Aggregator::Aggregator(size_t max_recent_events, int64_t window_minutes)
    : max_recent_events_(max_recent_events), window_minutes_(window_minutes) {
}

void Aggregator::process(const std::string& message) {
    // TODO: deserialize message, then call update methods
    DetectionEvent event = deserialize(message);
    update_class_counts(event);
    update_recent_events(event);
    update_timeline(event);
}

const AggregatedState& Aggregator::state() const {
    return state_;
}

void Aggregator::reset() {
    state_.class_counts.clear();
    state_.recent_events.clear();
    state_.timeline.clear();
}

DetectionEvent Aggregator::deserialize(const std::string& message) {
    // TODO: parse JSON string into DetectionEvent using nlohmann::json
    
    nlohmann::json parsed_json = nlohmann::json::parse(message);

    // Accessing values securely
    uint32_t frame_number = parsed_json["frame_number"];
    int64_t timestamp = parsed_json["timestamp"];
    std::vector<Detection> detections;
    for (const auto& d : parsed_json["detections"]) {
        Detection det;
        det.class_label = d["class"].get<std::string>();
        det.confidence = d["confidence"].get<float>();
        det.bbox = BoundingBox{
            d["bbox"]["x"].get<int>(),
            d["bbox"]["y"].get<int>(),
            d["bbox"]["w"].get<int>(),
            d["bbox"]["h"].get<int>()
        };
      detections.push_back(det);
    }
    
    return DetectionEvent{frame_number, timestamp, detections};
}

void Aggregator::update_class_counts(const DetectionEvent& event) {
    // TODO: increment count for each detected class
    // {"detections":[{"bbox":{"h":516,"w":612,"x":804,"y":560},"class":"person","confidence":0.8297368288040161}]
    for (auto& detection : event.detections) {
        ++state_.class_counts[detection.class_label];
    }
}

void Aggregator::update_recent_events(const DetectionEvent& event) {
    // TODO: push event, pop oldest if over max_recent_events_
    state_.recent_events.push_back(event);
    if (state_.recent_events.size() > max_recent_events_) {
        state_.recent_events.erase(state_.recent_events.begin());
    }
}

void Aggregator::update_timeline(const DetectionEvent& event) {
    int64_t minute = event.timestamp / 60000;

    // count detections per class for this event
    std::unordered_map<std::string, int> class_count;
    for (const auto& det : event.detections) {
        ++class_count[det.class_label];
    }

    // update timeline for each detected class
    for (const auto& [label, count] : class_count) {
        auto& deque = state_.timeline[label];

        if (!deque.empty() && deque.back().first == minute) {
            // same minute bucket — increment
            deque.back().second += count;
        } else {
            // new minute bucket — push
            deque.push_back({minute, count});
        }

        // evict entries outside the window
        while (!deque.empty() && deque.front().first < minute - window_minutes_) {
            deque.pop_front();
        }
    }
}

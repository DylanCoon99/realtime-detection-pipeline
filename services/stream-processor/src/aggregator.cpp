#include "aggregator.h"
#include "json.hpp"

Aggregator::Aggregator(size_t max_recent_events)
    : max_recent_events_(max_recent_events) {
}

void Aggregator::process(const std::string& message) {
    // TODO: deserialize message, then call update methods
}

const AggregatedState& Aggregator::state() const {
    return state_;
}

void Aggregator::reset() {
    // TODO: clear all state
}

DetectionEvent Aggregator::deserialize(const std::string& message) {
    // TODO: parse JSON string into DetectionEvent using nlohmann::json
    return DetectionEvent{};
}

void Aggregator::update_class_counts(const DetectionEvent& event) {
    // TODO: increment count for each detected class
}

void Aggregator::update_recent_events(const DetectionEvent& event) {
    // TODO: push event, pop oldest if over max_recent_events_
}

void Aggregator::update_timeline(const DetectionEvent& event) {
    // TODO: bucket detections by minute, increment per-class counts
}

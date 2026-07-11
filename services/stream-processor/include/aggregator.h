#pragma once

#include <string>
#include "types.h"

/*
    Aggregator — deserializes Kafka messages and maintains real-time aggregated state.

    Tracks:
    - Detection counts per class
    - Recent detection events (sliding window)
    - Detections per time bucket for timeline charts
*/

class Aggregator {
public:
    Aggregator(size_t max_recent_events = 100);

    // Deserialize a JSON message and update aggregated state
    void process(const std::string& message);

    // Get current aggregated state (for writing to Redis)
    const AggregatedState& state() const;

    // Reset all counters
    void reset();

private:
    // Parse a JSON string into a DetectionEvent
    DetectionEvent deserialize(const std::string& message);

    // Update internal state with a new event
    void update_class_counts(const DetectionEvent& event);
    void update_recent_events(const DetectionEvent& event);
    void update_timeline(const DetectionEvent& event);

    AggregatedState state_;
    size_t max_recent_events_;
};

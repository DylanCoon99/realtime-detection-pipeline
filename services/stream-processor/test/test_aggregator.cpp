#include <gtest/gtest.h>
#include "aggregator.h"
#include "json.hpp"

// Helper: build a JSON detection message
std::string make_message(uint32_t frame, int64_t timestamp,
                         std::vector<std::pair<std::string, float>> detections) {
    nlohmann::json j;
    j["frame_number"] = frame;
    j["timestamp"] = timestamp;
    j["detections"] = nlohmann::json::array();
    for (const auto& [label, conf] : detections) {
        j["detections"].push_back({
            {"class", label},
            {"confidence", conf},
            {"bbox", {{"x", 0}, {"y", 0}, {"w", 50}, {"h", 50}}}
        });
    }
    return j.dump();
}

// --- Deserialization ---

TEST(AggregatorTest, ProcessesValidMessage) {
    Aggregator agg;
    std::string msg = make_message(1, 60000, {{"person", 0.9f}});
    EXPECT_NO_THROW(agg.process(msg));
}

TEST(AggregatorTest, ThrowsOnInvalidJson) {
    Aggregator agg;
    EXPECT_THROW(agg.process("not json"), nlohmann::json::parse_error);
}

// --- Class counts ---

TEST(AggregatorTest, ClassCountsSingleDetection) {
    Aggregator agg;
    agg.process(make_message(1, 60000, {{"person", 0.9f}}));
    EXPECT_EQ(agg.state().class_counts.at("person"), 1);
}

TEST(AggregatorTest, ClassCountsAccumulate) {
    Aggregator agg;
    agg.process(make_message(1, 60000, {{"person", 0.9f}}));
    agg.process(make_message(2, 60001, {{"person", 0.8f}, {"car", 0.7f}}));
    EXPECT_EQ(agg.state().class_counts.at("person"), 2);
    EXPECT_EQ(agg.state().class_counts.at("car"), 1);
}

TEST(AggregatorTest, ClassCountsMultipleClassesPerFrame) {
    Aggregator agg;
    agg.process(make_message(1, 60000, {{"person", 0.9f}, {"person", 0.8f}, {"dog", 0.7f}}));
    EXPECT_EQ(agg.state().class_counts.at("person"), 2);
    EXPECT_EQ(agg.state().class_counts.at("dog"), 1);
}

// --- Recent events ---

TEST(AggregatorTest, RecentEventsStored) {
    Aggregator agg(100);
    agg.process(make_message(1, 60000, {{"person", 0.9f}}));
    agg.process(make_message(2, 60001, {{"car", 0.8f}}));
    EXPECT_EQ(agg.state().recent_events.size(), 2);
}

TEST(AggregatorTest, RecentEventsPreserveOrder) {
    Aggregator agg(100);
    agg.process(make_message(1, 60000, {{"person", 0.9f}}));
    agg.process(make_message(2, 60001, {{"car", 0.8f}}));
    EXPECT_EQ(agg.state().recent_events[0].frame_number, 1);
    EXPECT_EQ(agg.state().recent_events[1].frame_number, 2);
}

TEST(AggregatorTest, RecentEventsCappedAtMax) {
    Aggregator agg(3);
    agg.process(make_message(1, 60000, {{"person", 0.9f}}));
    agg.process(make_message(2, 60001, {{"person", 0.9f}}));
    agg.process(make_message(3, 60002, {{"person", 0.9f}}));
    agg.process(make_message(4, 60003, {{"person", 0.9f}}));
    EXPECT_EQ(agg.state().recent_events.size(), 3);
    // oldest (frame 1) should be evicted
    EXPECT_EQ(agg.state().recent_events[0].frame_number, 2);
}

// --- Timeline ---

TEST(AggregatorTest, TimelineCreatesEntry) {
    Aggregator agg(100, 60);
    agg.process(make_message(1, 60000, {{"person", 0.9f}}));
    EXPECT_EQ(agg.state().timeline.count("person"), 1);
    EXPECT_EQ(agg.state().timeline.at("person").size(), 1);
    EXPECT_EQ(agg.state().timeline.at("person")[0].second, 1);
}

TEST(AggregatorTest, TimelineSameMinuteBucketIncrements) {
    Aggregator agg(100, 60);
    // both timestamps in the same minute (minute bucket = 1)
    agg.process(make_message(1, 60000, {{"person", 0.9f}}));
    agg.process(make_message(2, 60500, {{"person", 0.8f}}));
    EXPECT_EQ(agg.state().timeline.at("person").size(), 1);
    EXPECT_EQ(agg.state().timeline.at("person")[0].second, 2);
}

TEST(AggregatorTest, TimelineDifferentMinutesBucketSeparately) {
    Aggregator agg(100, 60);
    // minute bucket 1 and minute bucket 2
    agg.process(make_message(1, 60000, {{"person", 0.9f}}));
    agg.process(make_message(2, 120000, {{"person", 0.8f}}));
    EXPECT_EQ(agg.state().timeline.at("person").size(), 2);
}

TEST(AggregatorTest, TimelineEvictsOldEntries) {
    Aggregator agg(100, 2);  // 2 minute window
    // minute 1
    agg.process(make_message(1, 60000, {{"person", 0.9f}}));
    // minute 4 — minute 1 should be evicted (4 - 2 = 2 > 1)
    agg.process(make_message(2, 240000, {{"person", 0.8f}}));
    EXPECT_EQ(agg.state().timeline.at("person").size(), 1);
    EXPECT_EQ(agg.state().timeline.at("person")[0].first, 4);
}

TEST(AggregatorTest, TimelineMultipleClasses) {
    Aggregator agg(100, 60);
    agg.process(make_message(1, 60000, {{"person", 0.9f}, {"car", 0.7f}}));
    EXPECT_EQ(agg.state().timeline.count("person"), 1);
    EXPECT_EQ(agg.state().timeline.count("car"), 1);
}

// --- Reset ---

TEST(AggregatorTest, ResetClearsAllState) {
    Aggregator agg;
    agg.process(make_message(1, 60000, {{"person", 0.9f}}));
    agg.reset();
    EXPECT_TRUE(agg.state().class_counts.empty());
    EXPECT_TRUE(agg.state().recent_events.empty());
    EXPECT_TRUE(agg.state().timeline.empty());
}

// --- Empty input ---

TEST(AggregatorTest, MessageWithNoDetections) {
    Aggregator agg;
    agg.process(make_message(1, 60000, {}));
    EXPECT_TRUE(agg.state().class_counts.empty());
    EXPECT_EQ(agg.state().recent_events.size(), 1);
    EXPECT_TRUE(agg.state().timeline.empty());
}

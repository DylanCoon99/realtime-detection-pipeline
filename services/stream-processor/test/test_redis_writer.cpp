#include <gtest/gtest.h>
#include "redis_writer.h"
#include "json.hpp"

/*
    Redis writer tests require a running Redis instance.
    Run: docker compose up -d redis
    These are integration tests, not pure unit tests.
*/

// --- Connection ---

TEST(RedisWriterTest, ConnectsToRedis) {
    EXPECT_NO_THROW(RedisWriter("localhost", 6379));
}

TEST(RedisWriterTest, ThrowsOnBadHost) {
    EXPECT_THROW(RedisWriter("nonexistent-host", 6379), std::runtime_error);
}

TEST(RedisWriterTest, IsConnected) {
    RedisWriter writer("localhost", 6379);
    EXPECT_TRUE(writer.is_connected());
}

// --- Write class counts ---

TEST(RedisWriterTest, WritesClassCounts) {
    RedisWriter writer("localhost", 6379);
    std::unordered_map<std::string, int> counts = {{"person", 10}, {"car", 5}};
    EXPECT_NO_THROW(writer.write_class_counts(counts));
}

// --- Write recent events ---

TEST(RedisWriterTest, WritesRecentEvents) {
    RedisWriter writer("localhost", 6379);
    std::vector<DetectionEvent> events = {
        {1, 60000, {{"person", 0.9f, {100, 200, 50, 50}}}},
        {2, 60001, {{"car", 0.8f, {300, 400, 80, 60}}}}
    };
    EXPECT_NO_THROW(writer.write_recent_events(events));
}

// --- Write timeline ---

TEST(RedisWriterTest, WritesTimeline) {
    RedisWriter writer("localhost", 6379);
    std::unordered_map<std::string, std::deque<std::pair<int64_t, int>>> timeline = {
        {"person", {{1, 5}, {2, 3}, {3, 7}}},
        {"car", {{1, 2}, {2, 1}}}
    };
    EXPECT_NO_THROW(writer.write_timeline(timeline));
}

// --- Write full state ---

TEST(RedisWriterTest, WritesFullState) {
    RedisWriter writer("localhost", 6379);
    AggregatedState state;
    state.class_counts = {{"person", 10}};
    state.recent_events = {{1, 60000, {{"person", 0.9f, {0, 0, 50, 50}}}}};
    state.timeline = {{"person", {{1, 5}}}};
    EXPECT_NO_THROW(writer.write(state));
}

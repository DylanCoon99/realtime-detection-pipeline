#include <gtest/gtest.h>
#include "frame_source.h"

// CMake defines TEST_FIXTURES_DIR; fall back to absolute path
#ifndef TEST_FIXTURES_DIR
#define TEST_FIXTURES_DIR "/Users/Dylan/Documents/realtime-detection-pipeline/services/capture-inference/test/fixtures"
#endif
const std::string TEST_VIDEO = std::string(TEST_FIXTURES_DIR) + "/test_video.mp4";

// Construction

TEST(SourceTest, OpensValidVideoFile) {
    EXPECT_NO_THROW(Source("test", TEST_VIDEO));
}

TEST(SourceTest, ThrowsOnInvalidPath) {
    EXPECT_THROW(Source("bad", "nonexistent.mp4"), std::runtime_error);
}

TEST(SourceTest, ThrowsOnInvalidCameraId) {
    EXPECT_THROW(Source("bad", 999), std::runtime_error);
}

TEST(SourceTest, NameReturnsSourceName) {
    Source src("my_video", TEST_VIDEO);
    EXPECT_EQ(src.name(), "my_video");
}

// process()

TEST(SourceTest, FirstFrameIsNotEmpty) {
    Source src("test", TEST_VIDEO);
    Frame frame = src.process();
    EXPECT_FALSE(frame.image.empty());
}

TEST(SourceTest, FrameHasThreeChannels) {
    Source src("test", TEST_VIDEO);
    Frame frame = src.process();
    EXPECT_EQ(frame.image.channels(), 3);
}

TEST(SourceTest, FrameHasValidDimensions) {
    Source src("test", TEST_VIDEO);
    Frame frame = src.process();
    EXPECT_GT(frame.image.rows, 0);
    EXPECT_GT(frame.image.cols, 0);
}

TEST(SourceTest, FrameNumberStartsAtZero) {
    Source src("test", TEST_VIDEO);
    Frame frame = src.process();
    EXPECT_EQ(frame.metadata.frame_number, 0);
}

TEST(SourceTest, FrameNumberIncrements) {
    Source src("test", TEST_VIDEO);
    Frame f0 = src.process();
    Frame f1 = src.process();
    Frame f2 = src.process();
    EXPECT_EQ(f0.metadata.frame_number, 0);
    EXPECT_EQ(f1.metadata.frame_number, 1);
    EXPECT_EQ(f2.metadata.frame_number, 2);
}

TEST(SourceTest, TimestampIsPositive) {
    Source src("test", TEST_VIDEO);
    Frame frame = src.process();
    EXPECT_GT(frame.metadata.timestamp, 0);
}

TEST(SourceTest, TimestampIncreases) {
    Source src("test", TEST_VIDEO);
    Frame f0 = src.process();
    Frame f1 = src.process();
    EXPECT_GE(f1.metadata.timestamp, f0.metadata.timestamp);
}

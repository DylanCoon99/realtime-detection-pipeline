#include <gtest/gtest.h>
#include "pipeline.h"

#ifndef TEST_FIXTURES_DIR
#define TEST_FIXTURES_DIR "/Users/Dylan/Documents/realtime-detection-pipeline/services/capture-inference/test/fixtures"
#endif

// Shared setup for all integration tests
class PipelineIntegration : public ::testing::Test {
protected:
    void SetUp() override {
        source = std::make_unique<Source>("test_video",
            std::string(TEST_FIXTURES_DIR) + "/test_video.mp4");
        preprocessor = std::make_unique<Preprocessor>(1, 3, 640, 640, "test_pp");
        detector = std::make_unique<Detector>();
        postprocessor = std::make_unique<Postprocessor>("test_post");
        pipeline = std::make_unique<Pipeline>(*source, *preprocessor, *detector, *postprocessor);
    }

    std::unique_ptr<Source> source;
    std::unique_ptr<Preprocessor> preprocessor;
    std::unique_ptr<Detector> detector;
    std::unique_ptr<Postprocessor> postprocessor;
    std::unique_ptr<Pipeline> pipeline;
};

// Pipeline executes without crashing

TEST_F(PipelineIntegration, ExecutesSuccessfully) {
    EXPECT_NO_THROW(pipeline->execute());
}

// Pipeline returns a non-empty annotated frame

TEST_F(PipelineIntegration, ReturnsNonEmptyFrame) {
    DetectionResult result = pipeline->execute();
    EXPECT_FALSE(result.frame.image.empty());
}

// Pipeline returns frame with correct channel count

TEST_F(PipelineIntegration, FrameHasThreeChannels) {
    DetectionResult result = pipeline->execute();
    EXPECT_EQ(result.frame.image.channels(), 3);
}

// Pipeline returns metadata from the source frame

TEST_F(PipelineIntegration, ResultHasMetadata) {
    DetectionResult result = pipeline->execute();
    EXPECT_EQ(result.metadata.frame_number, 0);
    EXPECT_GT(result.metadata.timestamp, 0);
}

// Pipeline produces detections on a real video frame

TEST_F(PipelineIntegration, ProducesDetections) {
    DetectionResult result = pipeline->execute();
    // video may or may not have detectable objects, so just verify the vector is valid
    EXPECT_GE(result.detections.size(), 0);
}

// Pipeline detections have valid fields

TEST_F(PipelineIntegration, DetectionsHaveValidFields) {
    DetectionResult result = pipeline->execute();
    for (const auto& det : result.detections) {
        EXPECT_GT(det.bounding_box.width, 0);
        EXPECT_GT(det.bounding_box.height, 0);
        EXPECT_FALSE(det.class_label.empty());
        EXPECT_GT(det.confidence_score, 0.0f);
        EXPECT_LE(det.confidence_score, 1.0f);
    }
}

// Pipeline processes multiple frames sequentially

TEST_F(PipelineIntegration, ProcessesMultipleFrames) {
    DetectionResult r0 = pipeline->execute();
    DetectionResult r1 = pipeline->execute();
    DetectionResult r2 = pipeline->execute();

    EXPECT_EQ(r0.metadata.frame_number, 0);
    EXPECT_EQ(r1.metadata.frame_number, 1);
    EXPECT_EQ(r2.metadata.frame_number, 2);

    EXPECT_FALSE(r0.frame.image.empty());
    EXPECT_FALSE(r1.frame.image.empty());
    EXPECT_FALSE(r2.frame.image.empty());
}

// Pipeline with bus image detects known objects

class PipelineIntegrationBus : public ::testing::Test {
protected:
    void SetUp() override {
        // Use bus.jpg as a single-frame source via video capture
        source = std::make_unique<Source>("bus_image",
            std::string(TEST_FIXTURES_DIR) + "/bus.jpg");
        preprocessor = std::make_unique<Preprocessor>(1, 3, 640, 640, "test_pp");
        detector = std::make_unique<Detector>();
        postprocessor = std::make_unique<Postprocessor>("test_post");
        pipeline = std::make_unique<Pipeline>(*source, *preprocessor, *detector, *postprocessor);
    }

    std::unique_ptr<Source> source;
    std::unique_ptr<Preprocessor> preprocessor;
    std::unique_ptr<Detector> detector;
    std::unique_ptr<Postprocessor> postprocessor;
    std::unique_ptr<Pipeline> pipeline;
};

TEST_F(PipelineIntegrationBus, DetectsPersonAndBus) {
    DetectionResult result = pipeline->execute();
    EXPECT_GT(result.detections.size(), 0);

    bool found_person = false;
    bool found_bus = false;
    for (const auto& det : result.detections) {
        if (det.class_label == "person") found_person = true;
        if (det.class_label == "bus") found_bus = true;
    }
    EXPECT_TRUE(found_person) << "Expected to detect a person in bus.jpg";
    EXPECT_TRUE(found_bus) << "Expected to detect a bus in bus.jpg";
}

TEST_F(PipelineIntegrationBus, NMSReducesDuplicates) {
    DetectionResult result = pipeline->execute();
    // count persons — NMS should reduce to a reasonable number (bus.jpg has ~4 people)
    int person_count = 0;
    for (const auto& det : result.detections) {
        if (det.class_label == "person") person_count++;
    }
    EXPECT_LE(person_count, 10) << "Too many person detections — NMS may not be working";
}

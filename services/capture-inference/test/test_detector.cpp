#include <gtest/gtest.h>
#include "types.h"
#include "preprocessor.h"
#include "detector.h"
#include <algorithm>

#ifndef TEST_FIXTURES_DIR
#define TEST_FIXTURES_DIR "/Users/Dylan/Documents/realtime-detection-pipeline/services/capture-inference/test/fixtures"
#endif

// Helper: load bus.jpg and preprocess into a TensorData
TensorData make_bus_tensor() {
    cv::Mat image = cv::imread(std::string(TEST_FIXTURES_DIR) + "/bus.jpg");
    Frame frame{image, FrameData{0, 0}};
    Preprocessor pp(1, 3, 640, 640, "test");
    return pp.process(frame);
}

// Helper: check if a class label exists in detections
bool has_class(const std::vector<DetectionObject>& detections, const std::string& label) {
    return std::any_of(detections.begin(), detections.end(),
        [&](const DetectionObject& d) { return d.class_label == label; });
}

// Construction

TEST(DetectorTest, ConstructsSuccessfully) {
    EXPECT_NO_THROW(Detector());
}

TEST(DetectorTest, ConstructsWithCustomThreshold) {
    EXPECT_NO_THROW(Detector(0.5f));
}

// Inference produces results

TEST(DetectorTest, DetectsObjectsInBusImage) {
    Detector detector;
    TensorData tensor = make_bus_tensor();
    auto detections = detector.process(tensor);
    EXPECT_GT(detections.size(), 0);
}

TEST(DetectorTest, DetectsPersonInBusImage) {
    Detector detector;
    TensorData tensor = make_bus_tensor();
    auto detections = detector.process(tensor);
    EXPECT_TRUE(has_class(detections, "person"));
}

TEST(DetectorTest, DetectsBusInBusImage) {
    Detector detector;
    TensorData tensor = make_bus_tensor();
    auto detections = detector.process(tensor);
    EXPECT_TRUE(has_class(detections, "bus"));
}

// Detection quality

TEST(DetectorTest, ConfidenceScoresAboveThreshold) {
    float threshold = 0.25f;
    Detector detector(threshold);
    TensorData tensor = make_bus_tensor();
    auto detections = detector.process(tensor);
    for (const auto& d : detections) {
        EXPECT_GT(d.confidence_score, threshold);
    }
}

TEST(DetectorTest, BoundingBoxesHavePositiveDimensions) {
    Detector detector;
    TensorData tensor = make_bus_tensor();
    auto detections = detector.process(tensor);
    for (const auto& d : detections) {
        EXPECT_GT(d.bounding_box.width, 0);
        EXPECT_GT(d.bounding_box.height, 0);
    }
}

TEST(DetectorTest, BoundingBoxesAreWithinFrame) {
    Detector detector;
    TensorData tensor = make_bus_tensor();
    auto detections = detector.process(tensor);
    for (const auto& d : detections) {
        EXPECT_GE(d.bounding_box.x, -d.bounding_box.width);
        EXPECT_GE(d.bounding_box.y, -d.bounding_box.height);
        EXPECT_LE(d.bounding_box.x + d.bounding_box.width, 640 * 2);
        EXPECT_LE(d.bounding_box.y + d.bounding_box.height, 640 * 2);
    }
}

// Threshold behavior

TEST(DetectorTest, HigherThresholdProducesFewerDetections) {
    TensorData tensor_low = make_bus_tensor();
    TensorData tensor_high = make_bus_tensor();

    Detector low_threshold(0.1f);
    Detector high_threshold(0.8f);

    auto detections_low = low_threshold.process(tensor_low);
    auto detections_high = high_threshold.process(tensor_high);

    EXPECT_GE(detections_low.size(), detections_high.size());
}

// Empty input

TEST(DetectorTest, BlankImageProducesNoHighConfidenceDetections) {
    cv::Mat blank(640, 640, CV_8UC3, cv::Scalar(0, 0, 0));
    Frame frame{blank, FrameData{0, 0}};
    Preprocessor pp(1, 3, 640, 640, "test");
    TensorData tensor = pp.process(frame);

    Detector detector(0.5f);
    auto detections = detector.process(tensor);
    EXPECT_EQ(detections.size(), 0);
}

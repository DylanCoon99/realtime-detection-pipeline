#include <gtest/gtest.h>
#include "types.h"
#include "postprocessor.h"

// Helper: create a blank frame
Frame make_blank_frame(int height = 640, int width = 640) {
    return Frame{
        cv::Mat(height, width, CV_8UC3, cv::Scalar(0, 0, 0)),
        FrameData{0, 0}
    };
}

// Helper: create a detection
DetectionObject make_detection(int x, int y, int w, int h,
                                const std::string& label, float confidence) {
    return DetectionObject{cv::Rect(x, y, w, h), label, confidence};
}

// NMS: no detections

TEST(PostprocessorTest, EmptyDetectionsReturnsEmpty) {
    Postprocessor pp("test");
    Frame frame = make_blank_frame();
    std::vector<DetectionObject> detections;
    pp.process(frame, detections);
    EXPECT_EQ(detections.size(), 0);
}

// NMS: single detection passes through

TEST(PostprocessorTest, SingleDetectionIsKept) {
    Postprocessor pp("test");
    Frame frame = make_blank_frame();
    std::vector<DetectionObject> detections = {
        make_detection(100, 100, 50, 50, "person", 0.9f)
    };
    pp.process(frame, detections);
    EXPECT_EQ(detections.size(), 1);
    EXPECT_EQ(detections[0].class_label, "person");
}

// NMS: non-overlapping boxes of same class are all kept

TEST(PostprocessorTest, NonOverlappingSameClassKept) {
    Postprocessor pp("test");
    Frame frame = make_blank_frame();
    std::vector<DetectionObject> detections = {
        make_detection(0, 0, 50, 50, "person", 0.9f),
        make_detection(400, 400, 50, 50, "person", 0.8f)
    };
    pp.process(frame, detections);
    EXPECT_EQ(detections.size(), 2);
}

// NMS: overlapping boxes of same class are suppressed

TEST(PostprocessorTest, OverlappingSameClassSuppressed) {
    Postprocessor pp("test");
    Frame frame = make_blank_frame();
    std::vector<DetectionObject> detections = {
        make_detection(100, 100, 100, 100, "person", 0.9f),
        make_detection(110, 110, 100, 100, "person", 0.7f),
        make_detection(105, 105, 100, 100, "person", 0.6f)
    };
    pp.process(frame, detections);
    EXPECT_EQ(detections.size(), 1);
    EXPECT_FLOAT_EQ(detections[0].confidence_score, 0.9f);
}

// NMS: overlapping boxes of different classes are both kept

TEST(PostprocessorTest, OverlappingDifferentClassesKept) {
    Postprocessor pp("test");
    Frame frame = make_blank_frame();
    std::vector<DetectionObject> detections = {
        make_detection(100, 100, 100, 100, "person", 0.9f),
        make_detection(110, 110, 100, 100, "bus", 0.8f)
    };
    pp.process(frame, detections);
    EXPECT_EQ(detections.size(), 2);
}

// NMS: highest confidence box is kept

TEST(PostprocessorTest, HighestConfidenceKept) {
    Postprocessor pp("test");
    Frame frame = make_blank_frame();
    std::vector<DetectionObject> detections = {
        make_detection(100, 100, 100, 100, "cat", 0.5f),
        make_detection(105, 105, 100, 100, "cat", 0.95f),
        make_detection(110, 110, 100, 100, "cat", 0.3f)
    };
    pp.process(frame, detections);
    EXPECT_EQ(detections.size(), 1);
    EXPECT_FLOAT_EQ(detections[0].confidence_score, 0.95f);
}

// NMS: custom threshold

TEST(PostprocessorTest, StrictThresholdSuppressesMore) {
    Frame frame1 = make_blank_frame();
    Frame frame2 = make_blank_frame();

    // partially overlapping boxes
    std::vector<DetectionObject> det_loose = {
        make_detection(100, 100, 100, 100, "person", 0.9f),
        make_detection(150, 150, 100, 100, "person", 0.8f)
    };
    std::vector<DetectionObject> det_strict = det_loose;

    Postprocessor loose("test", 0.8f);
    Postprocessor strict("test", 0.1f);

    loose.process(frame1, det_loose);
    strict.process(frame2, det_strict);

    EXPECT_GE(det_loose.size(), det_strict.size());
}

// Drawing: boxes are drawn on the frame

TEST(PostprocessorTest, DrawsOnFrame) {
    Frame frame = make_blank_frame();
    cv::Mat before = frame.image.clone();
    std::vector<DetectionObject> detections = {
        make_detection(100, 100, 50, 50, "person", 0.9f)
    };
    Postprocessor pp("test");
    pp.process(frame, detections);

    // frame should be modified (not identical to the blank original)
    bool changed = cv::norm(before, frame.image) > 0;
    EXPECT_TRUE(changed);
}

// Drawing: empty detections don't modify the frame

TEST(PostprocessorTest, NoDetectionsFrameUnchanged) {
    Frame frame = make_blank_frame();
    cv::Mat before = frame.image.clone();
    std::vector<DetectionObject> detections;
    Postprocessor pp("test");
    pp.process(frame, detections);

    bool unchanged = cv::norm(before, frame.image) == 0;
    EXPECT_TRUE(unchanged);
}

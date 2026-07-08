#include <gtest/gtest.h>
#include "preprocessor.h"
#include "types.h"

// Helper to create a synthetic frame with known pixel values
Frame make_test_frame(int height, int width, cv::Scalar color) {
    return Frame{
        cv::Mat(height, width, CV_8UC3, color),
        FrameData{0, 0}
    };
}

TEST(PreprocessorTest, OutputShapeIsCorrect) {
    Preprocessor pp(1, 3, 640, 640, "test");
    Frame frame = make_test_frame(480, 640, cv::Scalar(128, 64, 32));
    TensorData tensor = pp.process(frame);
    ASSERT_EQ(tensor.shape.size(), 4);
    EXPECT_EQ(tensor.shape[0], 1);
    EXPECT_EQ(tensor.shape[1], 3);
    EXPECT_EQ(tensor.shape[2], 640);
    EXPECT_EQ(tensor.shape[3], 640);
}

TEST(PreprocessorTest, OutputDataSizeMatchesShape) {
    Preprocessor pp(1, 3, 640, 640, "test");
    Frame frame = make_test_frame(480, 640, cv::Scalar(128, 64, 32));
    TensorData tensor = pp.process(frame);
    size_t expected = 1 * 3 * 640 * 640;
    EXPECT_EQ(tensor.data.size(), expected);
}

TEST(PreprocessorTest, ValuesAreNormalized) {
    Preprocessor pp(1, 3, 640, 640, "test");
    Frame frame = make_test_frame(480, 640, cv::Scalar(128, 64, 32));
    TensorData tensor = pp.process(frame);
    for (size_t i = 0; i < tensor.data.size(); i++) {
        EXPECT_GE(tensor.data[i], 0.0f) << "Value below 0 at index " << i;
        EXPECT_LE(tensor.data[i], 1.0f) << "Value above 1 at index " << i;
    }
}

TEST(PreprocessorTest, KnownPixelValuesNormalizeCorrectly) {
    // BGR (128, 64, 32) -> RGB (32, 64, 128) -> normalized (32/255, 64/255, 128/255)
    Preprocessor pp(1, 3, 640, 640, "test");
    Frame frame = make_test_frame(640, 640, cv::Scalar(128, 64, 32));
    TensorData tensor = pp.process(frame);

    // CHW layout: first 640*640 values are red channel
    float expected_r = 32.0f / 255.0f;
    float expected_g = 64.0f / 255.0f;
    float expected_b = 128.0f / 255.0f;
    size_t plane = 640 * 640;

    EXPECT_NEAR(tensor.data[0], expected_r, 1e-4f);          // R channel
    EXPECT_NEAR(tensor.data[plane], expected_g, 1e-4f);      // G channel
    EXPECT_NEAR(tensor.data[2 * plane], expected_b, 1e-4f);  // B channel
}

TEST(PreprocessorTest, HandlesSmallInput) {
    Preprocessor pp(1, 3, 640, 640, "test");
    Frame frame = make_test_frame(32, 32, cv::Scalar(100, 100, 100));
    TensorData tensor = pp.process(frame);
    EXPECT_EQ(tensor.data.size(), 1 * 3 * 640 * 640);
}

TEST(PreprocessorTest, HandlesLargeInput) {
    Preprocessor pp(1, 3, 640, 640, "test");
    Frame frame = make_test_frame(1920, 1080, cv::Scalar(200, 150, 100));
    TensorData tensor = pp.process(frame);
    EXPECT_EQ(tensor.data.size(), 1 * 3 * 640 * 640);
}

TEST(PreprocessorTest, HandlesNonSquareInput) {
    Preprocessor pp(1, 3, 640, 640, "test");
    Frame frame = make_test_frame(720, 1280, cv::Scalar(50, 50, 50));
    TensorData tensor = pp.process(frame);
    EXPECT_EQ(tensor.shape[2], 640);
    EXPECT_EQ(tensor.shape[3], 640);
}

TEST(PreprocessorTest, BlackImageProducesZeros) {
    Preprocessor pp(1, 3, 640, 640, "test");
    Frame frame = make_test_frame(640, 640, cv::Scalar(0, 0, 0));
    TensorData tensor = pp.process(frame);
    for (float val : tensor.data) {
        EXPECT_FLOAT_EQ(val, 0.0f);
    }
}

TEST(PreprocessorTest, WhiteImageProducesOnes) {
    Preprocessor pp(1, 3, 640, 640, "test");
    Frame frame = make_test_frame(640, 640, cv::Scalar(255, 255, 255));
    TensorData tensor = pp.process(frame);
    for (float val : tensor.data) {
        EXPECT_FLOAT_EQ(val, 1.0f);
    }
}

# capture-inference

C++ service that captures webcam frames, runs YOLOv8 inference via ONNX Runtime, and outputs detection results. Designed as a modular pipeline with independently testable stages.

## Pipeline Stages

```
FrameSource → Preprocessor → Detector → Postprocessor → Pipeline outputs
```

| Stage | Input | Output | Description |
|-------|-------|--------|-------------|
| `FrameSource` | webcam/video/image | `Frame` | Captures raw frames. Abstract interface allows swapping between webcam, video file, or test image. |
| `Preprocessor` | `Frame` | tensor buffer | Resize to 640x640, BGR→RGB, normalize to [0,1], HWC→CHW layout. Stateless. |
| `Detector` | tensor buffer | raw model output | Owns the ONNX Runtime session. Loads model, runs inference, returns raw output. |
| `Postprocessor` | raw model output | `vector<Detection>` | Confidence thresholding and non-maximum suppression (NMS). Produces clean detections. |
| `Pipeline` | — | `DetectionResult` | Orchestrates the above stages. Thin glue code, no business logic. |

## Data Types

Defined in `types.h`:

- **`Frame`** — wraps `cv::Mat` + metadata (timestamp, frame number)
- **`Detection`** — bounding box (x, y, w, h), class_id, confidence
- **`DetectionResult`** — original frame + vector of `Detection`s + inference time

## Design Decisions

**Separation of concerns.** Each stage has a single responsibility and can be tested independently. Swapping the model doesn't touch capture code. Swapping the camera doesn't touch inference code.

**Kafka is not a pipeline concern.** The pipeline is pure computer vision. Kafka publishing happens in `main.cpp`, which wires the pipeline output to a `KafkaPublisher`. This allows the pipeline to be run and tested without Kafka.

```cpp
// main.cpp (pseudocode)
auto source = WebcamSource(0);
auto pipeline = Pipeline(source, "models/yolov8n.onnx");
auto publisher = KafkaPublisher("localhost:9092", "detections");

while (running) {
    auto result = pipeline.run();
    publisher.publish(result);
}
```

**Synchronous loop to start.** On a Raspberry Pi 5, inference is the bottleneck (~5-10 FPS with YOLOv8n). A simple synchronous loop is the right starting point. Threading (producer-consumer with a frame queue) can be added later if capture blocks inference.

## Testing Strategy

| Test | Type | Dependencies |
|------|------|-------------|
| `test_preprocessor` | Unit | None — feed known image, assert output shape and value range |
| `test_postprocessor` | Unit | None — construct fake model output, assert correct detections and NMS behavior |
| `test_detector` | Integration | Requires model file — feed known tensor, assert output shape `(1, 84, 8400)` |
| `test_pipeline` | End-to-end | Requires model file — use test image as `FrameSource`, assert reasonable detections |

## Project Structure

```
capture-inference/
├── include/
│   ├── frame_source.h
│   ├── preprocessor.h
│   ├── detector.h
│   ├── postprocessor.h
│   ├── pipeline.h
│   └── types.h
├── src/
│   ├── frame_source.cpp
│   ├── preprocessor.cpp
│   ├── detector.cpp
│   ├── postprocessor.cpp
│   ├── pipeline.cpp
│   └── main.cpp
├── test/
│   ├── test_preprocessor.cpp
│   ├── test_postprocessor.cpp
│   └── test_pipeline.cpp
├── models/
│   └── yolov8n.onnx
└── CMakeLists.txt
```

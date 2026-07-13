# Implementation Plan

## Phase 1: Lab / Model Prep

- [x] Export YOLOv8 model to ONNX format
- [x] Experiment with OpenCV — basic image capture pipeline
- [x] Test model inference in ONNX Runtime environment

## Phase 2: capture-inference (C++)

- [x] Set up CMake project with src, include, test directories
- [x] Integrate OpenCV — capture frames from USB webcam
- [x] Integrate ONNX Runtime — load model and run inference on frames
- [x] Parse YOLO output — bounding boxes, class IDs, confidence scores
- [x] Integrate librdkafka — publish detection events to Kafka
- [x] Define detection event schema (JSON)
- [x] Add Google Test unit tests
- [x] MJPEG streamer — serve annotated frames over HTTP
- [x] Write Dockerfile
- [ ] Test on Raspberry Pi 5 with real webcam

## Phase 3: stream-processor (C++)

- [x] Set up CMake project with src, include, test directories
- [x] Integrate librdkafka — consume detection events from Kafka
- [x] Implement real-time aggregation logic (sliding window counts, etc.)
- [ ] Implement alert rules (e.g., person detected for N seconds)
- [x] Integrate Redis client — write hot data (recent detections, aggregates)
- [x] Add Google Test unit tests
- [ ] Write Dockerfile

## Phase 4: batch-processor (Python)

- [x] Set up project structure with requirements.txt
- [x] Implement Kafka consumer for detection events
- [x] Implement scheduled aggregation (hourly/daily summaries)
- [x] Integrate PostgreSQL — write historical aggregations
- [x] Create init.sql schema (detections + hourly_summaries tables)
- [ ] Add pytest tests
- [ ] Write Dockerfile

## Phase 5: api (Python/FastAPI)

- [x] Set up FastAPI project structure
- [x] REST endpoints — real-time detections, counts, timeline
- [x] Redis integration — serve real-time data
- [x] PostgreSQL integration — serve historical data
- [x] Add historical query endpoints (time range, hourly/daily summaries)
- [ ] Add pytest tests
- [ ] Write Dockerfile

## Phase 6: frontend (React/TypeScript)

- [x] Scaffold React/TypeScript project (Vite + Tailwind)
- [x] Live camera feed via MJPEG stream
- [x] Stream connection status indicator
- [x] Detection timeline view
- [x] Object class distribution charts
- [ ] Alert configuration UI
- [x] Write Dockerfile

## Phase 7: Infrastructure & Deployment

- [x] Set up Kafka (KRaft mode, no Zookeeper) in docker-compose
- [x] Set up Redis in docker-compose
- [x] Set up PostgreSQL in docker-compose
- [x] Verify Kafka — produce/consume test messages from CLI
- [ ] docker-compose.yml — wire all services together
- [ ] Caddyfile — reverse proxy with SSL
- [ ] prometheus.yml — metrics collection
- [ ] Grafana dashboards
- [ ] GitHub Actions CI/CD pipeline
- [ ] Deploy to Raspberry Pi 5

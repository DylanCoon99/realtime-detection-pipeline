# Real-Time Object Detection Pipeline

A self-hosted real-time video analytics system that runs YOLOv8 inference in C++ via ONNX Runtime, streams detection events through Apache Kafka, processes data with Python batch aggregation, and serves a live dashboard via React.

## Architecture

![Architecture Diagram](docs/architecture.png)

### Services

| Service | Language | Description |
|---------|----------|-------------|
| `capture-inference` | C++ | Captures webcam frames via OpenCV, runs YOLOv8 inference via ONNX Runtime, publishes detection events to Kafka |
| `stream-processor` | C++ | Kafka consumer that aggregates detections in real-time, applies alert rules, writes hot data to Redis |
| `batch-processor` | Python | Scheduled aggregation of historical detection data, writes to PostgreSQL |
| `api` | Python (FastAPI) | REST API serving detection data, WebSocket endpoint for live annotated video feed |
| `frontend` | React/TypeScript | Live camera feed with bounding box overlays, detection timeline, object class charts, alert configuration |

### Infrastructure

| Component | Purpose |
|-----------|---------|
| Apache Kafka | Event streaming between services |
| PostgreSQL | Persistent storage for historical detections and aggregations |
| Redis | Hot data cache for real-time aggregates and recent detections |
| Caddy | Reverse proxy with automatic SSL |
| Prometheus | Metrics collection from all services |
| Grafana | Monitoring dashboards and alerting |
| Docker Compose | Container orchestration for the full stack |
| GitHub Actions | CI/CD pipeline (test, build, deploy to home server) |

## Tech Stack

**Languages:** C++17, Python, TypeScript
**ML:** YOLOv8, ONNX Runtime
**Streaming:** Apache Kafka (librdkafka)
**Backend:** FastAPI, WebSockets
**Frontend:** React, TypeScript
**Databases:** PostgreSQL, Redis
**Infrastructure:** Docker, Docker Compose, GitHub Actions, Prometheus, Grafana, Caddy
**Build:** CMake, Google Test

## Project Structure

```
realtime-detection-pipeline/
├── services/
│   ├── capture-inference/     # C++ frame capture + YOLO inference
│   ├── stream-processor/      # C++ Kafka consumer + real-time aggregation
│   ├── batch-processor/       # Python batch aggregation
│   ├── api/                   # FastAPI REST + WebSocket server
│   └── frontend/              # React dashboard
├── docker-compose.yml
├── Caddyfile
├── prometheus.yml
├── docs/
│   └── architecture.png
├── .github/
│   └── workflows/
│       └── ci-cd.yml
└── README.md
```

## Development

### Prerequisites

- Docker and Docker Compose
- CMake 3.20+
- Python 3.11+
- Node.js 18+
- A USB webcam

### Local Development

```bash
# Start infrastructure services
docker compose up -d kafka zookeeper redis postgres

# Build and run C++ services locally
cd services/capture-inference
mkdir build && cd build
cmake .. && make -j$(nproc)
./capture-inference

# Run API
cd services/api
pip install -r requirements.txt
uvicorn main:app --reload

# Run frontend
cd services/frontend
npm install && npm run dev
```

### Full Stack (Docker)

```bash
docker compose up -d
```

### Running Tests

```bash
# C++ tests
cd services/capture-inference/build && ctest

# Python tests
cd services/api && pytest

# Integration tests
docker compose -f docker-compose.test.yml up --abort-on-container-exit
```

## Deployment

Self-hosted on a home server. GitHub Actions builds Docker images on push to `main`, pushes to GitHub Container Registry, and triggers a deploy via webhook.

## Status

**Pre-development.** Prerequisites: complete C++ build systems (CMake) and testing (Google Test) modules first.

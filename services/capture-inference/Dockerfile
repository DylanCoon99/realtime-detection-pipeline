# --- Build stage ---
FROM ubuntu:22.04 AS builder

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
    libopencv-dev \
    curl \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

# Install ONNX Runtime
ARG ONNXRT_VERSION=1.18.1
ARG TARGETARCH
RUN if [ "$TARGETARCH" = "arm64" ]; then \
        ONNXRT_ARCH="aarch64"; \
    else \
        ONNXRT_ARCH="x64"; \
    fi && \
    curl -L -o /tmp/onnxruntime.tgz \
        "https://github.com/microsoft/onnxruntime/releases/download/v${ONNXRT_VERSION}/onnxruntime-linux-${ONNXRT_ARCH}-${ONNXRT_VERSION}.tgz" && \
    tar -xzf /tmp/onnxruntime.tgz -C /opt && \
    mv /opt/onnxruntime-linux-${ONNXRT_ARCH}-${ONNXRT_VERSION} /opt/onnxruntime && \
    rm /tmp/onnxruntime.tgz

ENV ONNXRUNTIME_ROOT=/opt/onnxruntime
ENV CMAKE_PREFIX_PATH=/opt/onnxruntime

WORKDIR /app

# Copy source
COPY CMakeLists.txt .
COPY src/ src/
COPY include/ include/
COPY third_party/ third_party/

# Build (skip tests in Docker)
RUN mkdir build && cd build && \
    cmake .. \
        -DCMAKE_BUILD_TYPE=Release \
        -DBUILD_TESTS=OFF \
        -DONNXRUNTIME_LIB=/opt/onnxruntime/lib/libonnxruntime.so \
        -DONNXRUNTIME_INCLUDE_DIR=/opt/onnxruntime/include && \
    make -j$(nproc)

# --- Runtime stage ---
FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
    libopencv-core-dev \
    libopencv-imgproc-dev \
    libopencv-imgcodecs-dev \
    libopencv-videoio-dev \
    libopencv-dnn-dev \
    libopencv-highgui-dev \
    && rm -rf /var/lib/apt/lists/*

# Copy ONNX Runtime shared library
COPY --from=builder /opt/onnxruntime/lib/libonnxruntime.so* /usr/local/lib/
RUN ldconfig

# Copy built binary
COPY --from=builder /app/build/bin/capture-inference /usr/local/bin/

# Download model
RUN apt-get update && apt-get install -y --no-install-recommends curl ca-certificates && \
    mkdir -p /app/models && \
    curl -L -o /app/models/yolov8n.onnx \
        "https://github.com/ultralytics/assets/releases/download/v8.3.0/yolov8n.onnx" && \
    apt-get purge -y curl ca-certificates && apt-get autoremove -y && \
    rm -rf /var/lib/apt/lists/*

ENV MODEL_PATH=/app/models/yolov8n.onnx

EXPOSE 8081

CMD ["capture-inference"]

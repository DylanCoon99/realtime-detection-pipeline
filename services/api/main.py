from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware
import redis
import json

app = FastAPI(title="Detection Pipeline API")

app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_methods=["*"],
    allow_headers=["*"],
)

r = redis.Redis(host="localhost", port=6379, decode_responses=True)


@app.get("/health")
def health():
    try:
        r.ping()
        return {"status": "ok", "redis": "connected"}
    except redis.ConnectionError:
        return {"status": "error", "redis": "disconnected"}


@app.get("/api/detections/counts")
def get_class_counts():
    counts = r.hgetall("detections:class_counts")
    return {label: int(count) for label, count in counts.items()}


@app.get("/api/detections/recent")
def get_recent_detections():
    events = r.lrange("detections:recent", 0, -1)
    return [json.loads(e) for e in events]


@app.get("/api/detections/timeline")
def get_timeline():
    timeline = r.hgetall("detections:timeline")
    return {label: json.loads(buckets) for label, buckets in timeline.items()}

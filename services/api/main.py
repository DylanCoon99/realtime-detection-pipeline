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
    # TODO: check Redis connection, return status
    pass


@app.get("/api/detections/counts")
def get_class_counts():
    # TODO: read detections:class_counts hash from Redis
    pass


@app.get("/api/detections/recent")
def get_recent_detections():
    # TODO: read detections:recent list from Redis
    pass


@app.get("/api/detections/timeline")
def get_timeline():
    # TODO: read detections:timeline hash from Redis
    pass

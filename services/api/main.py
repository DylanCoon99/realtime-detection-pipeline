from fastapi import FastAPI, Query
from fastapi.middleware.cors import CORSMiddleware
import redis
import psycopg2
import psycopg2.extras
import json
from datetime import datetime

app = FastAPI(title="Detection Pipeline API")

app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_methods=["*"],
    allow_headers=["*"],
)

r = redis.Redis(host="localhost", port=6379, decode_responses=True)


def get_db():
    return psycopg2.connect(
        dbname="detections", user="postgres", password="postgres",
        host="localhost", port=5433
    )


# --- Health ---

@app.get("/health")
def health():
    status = {"redis": "disconnected", "postgres": "disconnected"}
    try:
        r.ping()
        status["redis"] = "connected"
    except redis.ConnectionError:
        pass
    try:
        conn = get_db()
        conn.close()
        status["postgres"] = "connected"
    except Exception:
        pass
    status["status"] = "ok" if all(v == "connected" for v in status.values() if v != status["status"]) else "degraded"
    return status


# --- Real-time endpoints (Redis) ---

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


# --- Historical endpoints (PostgreSQL) ---

@app.get("/api/detections/history")
def get_history(
    start: str = Query(..., description="Start time ISO format, e.g. 2026-07-12T00:00:00"),
    end: str = Query(..., description="End time ISO format, e.g. 2026-07-12T23:59:59"),
    class_label: str = Query(None, description="Filter by class label"),
    limit: int = Query(100, description="Max results"),
):
    conn = get_db()
    cursor = conn.cursor(cursor_factory=psycopg2.extras.RealDictCursor)

    start_ms = int(datetime.fromisoformat(start).timestamp() * 1000)
    end_ms = int(datetime.fromisoformat(end).timestamp() * 1000)

    if class_label:
        cursor.execute(
            """SELECT frame_number, timestamp, class_label, confidence, bbox_x, bbox_y, bbox_w, bbox_h
               FROM detections WHERE timestamp BETWEEN %s AND %s AND class_label = %s
               ORDER BY timestamp DESC LIMIT %s""",
            (start_ms, end_ms, class_label, limit)
        )
    else:
        cursor.execute(
            """SELECT frame_number, timestamp, class_label, confidence, bbox_x, bbox_y, bbox_w, bbox_h
               FROM detections WHERE timestamp BETWEEN %s AND %s
               ORDER BY timestamp DESC LIMIT %s""",
            (start_ms, end_ms, limit)
        )

    rows = cursor.fetchall()
    cursor.close()
    conn.close()
    return rows


@app.get("/api/detections/hourly")
def get_hourly(
    start: str = Query(..., description="Start time ISO format"),
    end: str = Query(..., description="End time ISO format"),
    class_label: str = Query(None, description="Filter by class label"),
):
    conn = get_db()
    cursor = conn.cursor(cursor_factory=psycopg2.extras.RealDictCursor)

    start_dt = datetime.fromisoformat(start)
    end_dt = datetime.fromisoformat(end)

    if class_label:
        cursor.execute(
            """SELECT hour, class_label, count, avg_confidence
               FROM hourly_summaries WHERE hour BETWEEN %s AND %s AND class_label = %s
               ORDER BY hour""",
            (start_dt, end_dt, class_label)
        )
    else:
        cursor.execute(
            """SELECT hour, class_label, count, avg_confidence
               FROM hourly_summaries WHERE hour BETWEEN %s AND %s
               ORDER BY hour""",
            (start_dt, end_dt)
        )

    rows = cursor.fetchall()
    cursor.close()
    conn.close()
    return rows

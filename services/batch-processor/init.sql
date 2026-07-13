CREATE TABLE IF NOT EXISTS detections (
    id            SERIAL PRIMARY KEY,
    frame_number  INTEGER NOT NULL,
    timestamp     BIGINT NOT NULL,
    class_label   VARCHAR(50) NOT NULL,
    confidence    FLOAT NOT NULL,
    bbox_x        INTEGER NOT NULL,
    bbox_y        INTEGER NOT NULL,
    bbox_w        INTEGER NOT NULL,
    bbox_h        INTEGER NOT NULL,
    created_at    TIMESTAMP DEFAULT NOW()
);

CREATE TABLE IF NOT EXISTS hourly_summaries (
    id             SERIAL PRIMARY KEY,
    hour           TIMESTAMP NOT NULL,
    class_label    VARCHAR(50) NOT NULL,
    count          INTEGER NOT NULL,
    avg_confidence FLOAT NOT NULL,
    UNIQUE(hour, class_label)
);

CREATE INDEX IF NOT EXISTS idx_detections_timestamp ON detections(timestamp);
CREATE INDEX IF NOT EXISTS idx_detections_class ON detections(class_label);
CREATE INDEX IF NOT EXISTS idx_hourly_hour ON hourly_summaries(hour);

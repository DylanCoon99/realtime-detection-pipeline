import psycopg2


class Database:
    def __init__(self, host: str, port: int, dbname: str, user: str, password: str):
        self.connection = None
        try:
            self.connection = psycopg2.connect(
                dbname=dbname,
                user=user,
                password=password,
                host=host,
                port=port
            )
            self.connection.autocommit = False
            print(f"[Database] Connected to PostgreSQL at {host}:{port}/{dbname}")
        except (Exception, psycopg2.Error) as error:
            raise RuntimeError(f"Failed to connect to PostgreSQL: {error}")

    def close(self):
        if self.connection:
            self.connection.close()
            print("[Database] PostgreSQL connection closed.")

    def insert_detections(self, frame_number: int, timestamp: int, detections: list):
        """Insert one row per detection from an event."""
        cursor = self.connection.cursor()

        for det in detections:
            cursor.execute(
                """INSERT INTO detections (frame_number, timestamp, class_label, confidence, bbox_x, bbox_y, bbox_w, bbox_h)
                   VALUES (%s, %s, %s, %s, %s, %s, %s, %s)""",
                (
                    frame_number,
                    timestamp,
                    det["class"],
                    det["confidence"],
                    det["bbox"]["x"],
                    det["bbox"]["y"],
                    det["bbox"]["w"],
                    det["bbox"]["h"],
                )
            )

        self.connection.commit()
        cursor.close()

    def aggregate_hourly(self):
        """Roll up raw detections older than 24h into hourly_summaries."""
        cursor = self.connection.cursor()

        cursor.execute("""
            INSERT INTO hourly_summaries (hour, class_label, count, avg_confidence)
            SELECT
                date_trunc('hour', created_at) AS hour,
                class_label,
                COUNT(*) AS count,
                AVG(confidence) AS avg_confidence
            FROM detections
            WHERE created_at < NOW() - INTERVAL '24 hours'
            GROUP BY hour, class_label
            ON CONFLICT DO NOTHING
        """)

        self.connection.commit()
        cursor.close()
        print("[Database] Hourly aggregation complete.")

    def cleanup_old_events(self, max_age_hours: int = 24):
        """Delete raw detection rows older than max_age_hours."""
        cursor = self.connection.cursor()

        cursor.execute(
            "DELETE FROM detections WHERE created_at < NOW() - INTERVAL '%s hours'",
            (max_age_hours,)
        )

        deleted = cursor.rowcount
        self.connection.commit()
        cursor.close()
        print(f"[Database] Cleaned up {deleted} old detection rows.")

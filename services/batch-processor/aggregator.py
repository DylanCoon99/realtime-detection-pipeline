import schedule
import threading
import time
from db import Database


class Aggregator:
    def __init__(self, db: Database):
        self.db = db

    def run_aggregation(self):
        """Run hourly aggregation and cleanup."""
        try:
            print("[Aggregator] Running scheduled aggregation...")
            self.db.aggregate_hourly()
            self.db.cleanup_old_events()
        except Exception as e:
            print(f"[Aggregator] Failed to run aggregation: {e}")

    def start_schedule(self):
        """Start the scheduled aggregation in a background thread."""
        schedule.every(1).hours.do(self.run_aggregation)

        def run_loop():
            while True:
                schedule.run_pending()
                time.sleep(60)

        thread = threading.Thread(target=run_loop, daemon=True)
        thread.start()
        print("[Aggregator] Scheduled hourly aggregation started.")

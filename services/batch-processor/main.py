import json
import threading
from kafka_consumer import BatchKafkaConsumer
from db import Database
from aggregator import Aggregator

def main():
    db = Database(host="localhost", port=5432, dbname="detections", user="postgres", password="postgres")
    consumer = BatchKafkaConsumer(broker="localhost:29092", topic="detections", group_id="batch-processor")
    aggregator = Aggregator(db)

    # Run scheduled aggregation in a background thread
    aggregator.start_schedule()

    # Consume and insert
    print("[main] Batch processor running...")
    # TODO: parse message and insert into PostgreSQL
    for event in consumer.consume():
        frame_number = event["frame_number"]                                                                                         
        timestamp = event["timestamp"]
        detections = []      
        for det in event["detections"]:                                                                                              
            class_label = det["class"]                                                                                             
            confidence = det["confidence"]                                                                                           
            bbox = det["bbox"]
            detections.append(det)
        # insert into PostgreSQL
        db.insert_detections(frame_number, timestamp, detections)


if __name__ == "__main__":
    main()

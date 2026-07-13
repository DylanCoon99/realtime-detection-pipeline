from kafka import KafkaConsumer
import json


class BatchKafkaConsumer:
    def __init__(self, broker: str, topic: str, group_id: str):
        self.consumer = KafkaConsumer(
            topic,
            bootstrap_servers=broker,
            group_id=group_id,
            auto_offset_reset="earliest",
            value_deserializer=lambda m: json.loads(m.decode("utf-8")),
        )
        print(f"[BatchKafkaConsumer] Subscribed to {topic} on {broker}")

    def consume(self):
        """Generator that yields parsed detection events."""
        for message in self.consumer:
            yield message.value

    def close(self):
        self.consumer.close()

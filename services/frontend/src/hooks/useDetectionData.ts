import { useState, useEffect } from 'react';

interface DetectionEvent {
  frame_number: number;
  timestamp: number;
  detections: {
    class: string;
    confidence: number;
    bbox: { x: number; y: number; w: number; h: number };
  }[];
}

interface TimelineBucket {
  minute: number;
  count: number;
}

interface DetectionData {
  classCounts: Record<string, number>;
  recentEvents: DetectionEvent[];
  timeline: Record<string, TimelineBucket[]>;
}

export function useDetectionData(pollInterval = 1000): DetectionData {
  const [classCounts, setClassCounts] = useState<Record<string, number>>({});
  const [recentEvents, setRecentEvents] = useState<DetectionEvent[]>([]);
  const [timeline, setTimeline] = useState<Record<string, TimelineBucket[]>>({});

  useEffect(() => {
    const fetchData = async () => {
      try {
        const [countsRes, recentRes, timelineRes] = await Promise.all([
          fetch('/api/detections/counts'),
          fetch('/api/detections/recent'),
          fetch('/api/detections/timeline'),
        ]);
        if (countsRes.ok) setClassCounts(await countsRes.json());
        if (recentRes.ok) setRecentEvents(await recentRes.json());
        if (timelineRes.ok) setTimeline(await timelineRes.json());
      } catch {
        // API not available, keep stale data
      }
    };

    fetchData();
    const interval = setInterval(fetchData, pollInterval);
    return () => clearInterval(interval);
  }, [pollInterval]);

  return { classCounts, recentEvents, timeline };
}

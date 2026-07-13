import { useState, useCallback } from 'react';

interface HistoricalDetection {
  frame_number: number;
  timestamp: number;
  class_label: string;
  confidence: number;
  bbox_x: number;
  bbox_y: number;
  bbox_w: number;
  bbox_h: number;
}

interface HourlySummary {
  hour: string;
  class_label: string;
  count: number;
  avg_confidence: number;
}

interface HistoricalData {
  detections: HistoricalDetection[];
  hourly: HourlySummary[];
  loading: boolean;
  fetchHistory: (start: string, end: string, classLabel?: string) => void;
  fetchHourly: (start: string, end: string, classLabel?: string) => void;
}

export function useHistoricalData(): HistoricalData {
  const [detections, setDetections] = useState<HistoricalDetection[]>([]);
  const [hourly, setHourly] = useState<HourlySummary[]>([]);
  const [loading, setLoading] = useState(false);

  const fetchHistory = useCallback(async (start: string, end: string, classLabel?: string) => {
    setLoading(true);
    try {
      let url = `/api/detections/history?start=${start}&end=${end}`;
      if (classLabel) url += `&class_label=${classLabel}`;
      const res = await fetch(url);
      if (res.ok) setDetections(await res.json());
    } catch {
      // API not available
    }
    setLoading(false);
  }, []);

  const fetchHourly = useCallback(async (start: string, end: string, classLabel?: string) => {
    setLoading(true);
    try {
      let url = `/api/detections/hourly?start=${start}&end=${end}`;
      if (classLabel) url += `&class_label=${classLabel}`;
      const res = await fetch(url);
      if (res.ok) setHourly(await res.json());
    } catch {
      // API not available
    }
    setLoading(false);
  }, []);

  return { detections, hourly, loading, fetchHistory, fetchHourly };
}

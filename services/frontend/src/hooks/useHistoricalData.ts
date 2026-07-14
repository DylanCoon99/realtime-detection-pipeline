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
  query: (start: string, end: string, classLabel?: string) => void;
}

export function useHistoricalData(): HistoricalData {
  const [detections, setDetections] = useState<HistoricalDetection[]>([]);
  const [hourly, setHourly] = useState<HourlySummary[]>([]);
  const [loading, setLoading] = useState(false);

  const query = useCallback(async (start: string, end: string, classLabel?: string) => {
    setLoading(true);
    try {
      // Append seconds if missing (datetime-local gives "2026-07-12T00:00")
      const s = start.length === 16 ? start + ':00' : start;
      const e = end.length === 16 ? end + ':00' : end;

      let historyUrl = `/api/detections/history?start=${s}&end=${e}`;
      let hourlyUrl = `/api/detections/hourly?start=${s}&end=${e}`;
      if (classLabel) {
        historyUrl += `&class_label=${classLabel}`;
        hourlyUrl += `&class_label=${classLabel}`;
      }

      const [historyRes, hourlyRes] = await Promise.all([
        fetch(historyUrl),
        fetch(hourlyUrl),
      ]);

      if (historyRes.ok) setDetections(await historyRes.json());
      if (hourlyRes.ok) setHourly(await hourlyRes.json());
    } catch {
      // API not available
    }
    setLoading(false);
  }, []);

  return { detections, hourly, loading, query };
}

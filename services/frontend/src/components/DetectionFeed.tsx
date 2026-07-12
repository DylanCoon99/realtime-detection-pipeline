interface DetectionEvent {
  frame_number: number;
  timestamp: number;
  detections: {
    class: string;
    confidence: number;
  }[];
}

interface DetectionFeedProps {
  events: DetectionEvent[];
}

export function DetectionFeed({ events }: DetectionFeedProps) {
  if (events.length === 0) {
    return (
      <div className="bg-gray-900 rounded-lg border border-gray-700 p-4 h-96 flex items-center justify-center">
        <p className="text-gray-600 text-sm">Waiting for detections...</p>
      </div>
    );
  }

  return (
    <div className="bg-gray-900 rounded-lg border border-gray-700 h-96 overflow-y-auto">
      {[...events].reverse().map((event, i) => (
        <div key={i} className="px-4 py-2 border-b border-gray-800 last:border-0">
          <div className="text-xs text-gray-500 mb-1">
            Frame {event.frame_number} &middot;{' '}
            {new Date(event.timestamp).toLocaleTimeString()}
          </div>
          {event.detections.map((det, j) => (
            <div key={j} className="flex justify-between text-sm">
              <span className="text-gray-300">{det.class}</span>
              <span className="text-gray-500">
                {Math.round(det.confidence * 100)}%
              </span>
            </div>
          ))}
        </div>
      ))}
    </div>
  );
}

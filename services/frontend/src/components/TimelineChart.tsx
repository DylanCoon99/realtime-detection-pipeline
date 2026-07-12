import {
  LineChart,
  Line,
  XAxis,
  YAxis,
  Tooltip,
  ResponsiveContainer,
  Legend,
} from 'recharts';

interface TimelineBucket {
  minute: number;
  count: number;
}

interface TimelineChartProps {
  timeline: Record<string, TimelineBucket[]>;
}

const COLORS = [
  '#3b82f6', '#22c55e', '#eab308', '#ef4444', '#a855f7',
  '#ec4899', '#06b6d4', '#f97316', '#14b8a6', '#8b5cf6',
];

export function TimelineChart({ timeline }: TimelineChartProps) {
  const classes = Object.keys(timeline);

  if (classes.length === 0) {
    return (
      <div className="bg-gray-900 rounded-lg border border-gray-700 p-4 h-64 flex items-center justify-center">
        <p className="text-gray-600 text-sm">Waiting for detections...</p>
      </div>
    );
  }

  // Merge all classes into a single dataset keyed by minute
  const minuteMap = new Map<number, Record<string, number>>();

  for (const [label, buckets] of Object.entries(timeline)) {
    for (const { minute, count } of buckets) {
      if (!minuteMap.has(minute)) {
        minuteMap.set(minute, {});
      }
      minuteMap.get(minute)![label] = count;
    }
  }

  const data = Array.from(minuteMap.entries())
    .sort(([a], [b]) => a - b)
    .map(([minute, counts]) => ({
      minute: new Date(minute * 60000).toLocaleTimeString([], {
        hour: '2-digit',
        minute: '2-digit',
      }),
      ...counts,
    }));

  return (
    <div className="bg-gray-900 rounded-lg border border-gray-700 p-4 h-64">
      <ResponsiveContainer width="100%" height="100%">
        <LineChart data={data}>
          <XAxis dataKey="minute" stroke="#6b7280" fontSize={12} />
          <YAxis stroke="#6b7280" fontSize={12} />
          <Tooltip
            contentStyle={{ backgroundColor: '#1f2937', border: '1px solid #374151' }}
            labelStyle={{ color: '#d1d5db' }}
          />
          <Legend />
          {classes.map((label, i) => (
            <Line
              key={label}
              type="monotone"
              dataKey={label}
              stroke={COLORS[i % COLORS.length]}
              strokeWidth={2}
              dot={false}
            />
          ))}
        </LineChart>
      </ResponsiveContainer>
    </div>
  );
}

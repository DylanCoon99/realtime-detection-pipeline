import {
  BarChart,
  Bar,
  XAxis,
  YAxis,
  Tooltip,
  ResponsiveContainer,
  Cell,
} from 'recharts';

interface ClassChartProps {
  counts: Record<string, number>;
}

const COLORS = [
  '#3b82f6', '#22c55e', '#eab308', '#ef4444', '#a855f7',
  '#ec4899', '#06b6d4', '#f97316', '#14b8a6', '#8b5cf6',
];

export function ClassChart({ counts }: ClassChartProps) {
  const data = Object.entries(counts)
    .map(([name, count]) => ({ name, count }))
    .sort((a, b) => b.count - a.count);

  if (data.length === 0) {
    return (
      <div className="bg-gray-900 rounded-lg border border-gray-700 p-4 h-64 flex items-center justify-center">
        <p className="text-gray-600 text-sm">Waiting for detections...</p>
      </div>
    );
  }

  return (
    <div className="bg-gray-900 rounded-lg border border-gray-700 p-4 h-64">
      <ResponsiveContainer width="100%" height="100%">
        <BarChart data={data} layout="vertical" margin={{ left: 60 }}>
          <XAxis type="number" stroke="#6b7280" fontSize={12} />
          <YAxis type="category" dataKey="name" stroke="#6b7280" fontSize={12} />
          <Tooltip
            contentStyle={{ backgroundColor: '#1f2937', border: '1px solid #374151' }}
            labelStyle={{ color: '#d1d5db' }}
          />
          <Bar dataKey="count" radius={[0, 4, 4, 0]}>
            {data.map((_, i) => (
              <Cell key={i} fill={COLORS[i % COLORS.length]} />
            ))}
          </Bar>
        </BarChart>
      </ResponsiveContainer>
    </div>
  );
}

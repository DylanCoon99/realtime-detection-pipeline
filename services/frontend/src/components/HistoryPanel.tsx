import { useState } from 'react';
import { useHistoricalData } from '../hooks/useHistoricalData';
import {
  BarChart,
  Bar,
  XAxis,
  YAxis,
  Tooltip,
  ResponsiveContainer,
  Cell,
} from 'recharts';

const COLORS = [
  '#3b82f6', '#22c55e', '#eab308', '#ef4444', '#a855f7',
  '#ec4899', '#06b6d4', '#f97316', '#14b8a6', '#8b5cf6',
];

export function HistoryPanel() {
  const { detections, hourly, loading, query } = useHistoricalData();
  const [start, setStart] = useState('');
  const [end, setEnd] = useState('');
  const [classFilter, setClassFilter] = useState('');

  const handleQuery = () => {
    if (!start || !end) return;
    query(start, end, classFilter || undefined);
  };

  // Aggregate from raw detections if no hourly summaries exist yet
  const countSource = hourly.length > 0
    ? hourly.reduce<Record<string, number>>((acc, row) => {
        acc[row.class_label] = (acc[row.class_label] || 0) + row.count;
        return acc;
      }, {})
    : detections.reduce<Record<string, number>>((acc, row) => {
        acc[row.class_label] = (acc[row.class_label] || 0) + 1;
        return acc;
      }, {});

  const chartData = Object.entries(countSource)
    .map(([name, count]) => ({ name, count }))
    .sort((a, b) => b.count - a.count);

  return (
    <div className="space-y-4">
      {/* Query controls */}
      <div className="bg-gray-900 rounded-lg border border-gray-700 p-4">
        <div className="flex flex-wrap gap-3 items-end">
          <div>
            <label className="block text-xs text-gray-400 mb-1">Start</label>
            <input
              type="datetime-local"
              value={start}
              onChange={(e) => setStart(e.target.value)}
              className="bg-gray-800 border border-gray-600 rounded px-3 py-1.5 text-sm text-white"
            />
          </div>
          <div>
            <label className="block text-xs text-gray-400 mb-1">End</label>
            <input
              type="datetime-local"
              value={end}
              onChange={(e) => setEnd(e.target.value)}
              className="bg-gray-800 border border-gray-600 rounded px-3 py-1.5 text-sm text-white"
            />
          </div>
          <div>
            <label className="block text-xs text-gray-400 mb-1">Class (optional)</label>
            <input
              type="text"
              value={classFilter}
              onChange={(e) => setClassFilter(e.target.value)}
              placeholder="e.g. person"
              className="bg-gray-800 border border-gray-600 rounded px-3 py-1.5 text-sm text-white"
            />
          </div>
          <button
            onClick={handleQuery}
            disabled={loading}
            className="bg-blue-600 hover:bg-blue-700 text-white text-sm px-4 py-1.5 rounded disabled:opacity-50"
          >
            {loading ? 'Loading...' : 'Query'}
          </button>
        </div>
      </div>

      <div className="grid grid-cols-1 md:grid-cols-2 gap-6">
        {/* Hourly summary chart */}
        <div>
          <h3 className="text-sm font-medium text-gray-400 uppercase tracking-wider mb-3">
            Historical Class Distribution
          </h3>
          <div className="bg-gray-900 rounded-lg border border-gray-700 p-4 h-64">
            {chartData.length > 0 ? (
              <ResponsiveContainer width="100%" height="100%">
                <BarChart data={chartData} layout="vertical" margin={{ left: 60 }}>
                  <XAxis type="number" stroke="#6b7280" fontSize={12} />
                  <YAxis type="category" dataKey="name" stroke="#6b7280" fontSize={12} />
                  <Tooltip
                    contentStyle={{ backgroundColor: '#1f2937', border: '1px solid #374151' }}
                    labelStyle={{ color: '#d1d5db' }}
                  />
                  <Bar dataKey="count" radius={[0, 4, 4, 0]}>
                    {chartData.map((_, i) => (
                      <Cell key={i} fill={COLORS[i % COLORS.length]} />
                    ))}
                  </Bar>
                </BarChart>
              </ResponsiveContainer>
            ) : (
              <div className="h-full flex items-center justify-center text-gray-600 text-sm">
                Run a query to see historical data
              </div>
            )}
          </div>
        </div>

        {/* Detection event list */}
        <div>
          <h3 className="text-sm font-medium text-gray-400 uppercase tracking-wider mb-3">
            Historical Events ({detections.length})
          </h3>
          <div className="bg-gray-900 rounded-lg border border-gray-700 h-64 overflow-y-auto">
            {detections.length > 0 ? (
              detections.map((det, i) => (
                <div key={i} className="px-4 py-2 border-b border-gray-800 last:border-0">
                  <div className="flex justify-between text-sm">
                    <span className="text-gray-300">{det.class_label}</span>
                    <span className="text-gray-500">{Math.round(det.confidence * 100)}%</span>
                  </div>
                  <div className="text-xs text-gray-500">
                    Frame {det.frame_number} &middot;{' '}
                    {new Date(det.timestamp).toLocaleString()}
                  </div>
                </div>
              ))
            ) : (
              <div className="h-full flex items-center justify-center text-gray-600 text-sm">
                Run a query to see historical events
              </div>
            )}
          </div>
        </div>
      </div>
    </div>
  );
}

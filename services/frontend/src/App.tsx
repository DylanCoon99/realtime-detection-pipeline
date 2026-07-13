import { VideoFeed } from './components/VideoFeed';
import { StatusIndicator } from './components/StatusIndicator';
import { DetectionFeed } from './components/DetectionFeed';
import { ClassChart } from './components/ClassChart';
import { TimelineChart } from './components/TimelineChart';
import { HistoryPanel } from './components/HistoryPanel';
import { useStreamStatus } from './hooks/useStreamStatus';
import { useDetectionData } from './hooks/useDetectionData';

function App() {
  const connected = useStreamStatus();
  const { classCounts, recentEvents, timeline } = useDetectionData();

  return (
    <div className="min-h-screen bg-gray-950 text-white">
      {/* Header */}
      <header className="border-b border-gray-800 px-6 py-4 flex items-center justify-between">
        <h1 className="text-xl font-semibold tracking-tight">
          Real-Time Detection Dashboard
        </h1>
        <StatusIndicator connected={connected} />
      </header>

      {/* Main Content */}
      <main className="max-w-7xl mx-auto p-6">
        {/* Live section */}
        <div className="grid grid-cols-1 lg:grid-cols-3 gap-6">
          {/* Video Feed */}
          <div className="lg:col-span-2">
            <h2 className="text-sm font-medium text-gray-400 uppercase tracking-wider mb-3">
              Live Feed
            </h2>
            <VideoFeed connected={connected} />
          </div>

          {/* Recent Detections */}
          <div>
            <h2 className="text-sm font-medium text-gray-400 uppercase tracking-wider mb-3">
              Recent Detections
            </h2>
            <DetectionFeed events={recentEvents} />
          </div>
        </div>

        {/* Real-time charts */}
        <div className="grid grid-cols-1 md:grid-cols-2 gap-6 mt-6">
          <div>
            <h2 className="text-sm font-medium text-gray-400 uppercase tracking-wider mb-3">
              Class Distribution
            </h2>
            <ClassChart counts={classCounts} />
          </div>
          <div>
            <h2 className="text-sm font-medium text-gray-400 uppercase tracking-wider mb-3">
              Detections Over Time
            </h2>
            <TimelineChart timeline={timeline} />
          </div>
        </div>

        {/* Historical section */}
        <div className="mt-10 border-t border-gray-800 pt-6">
          <h2 className="text-lg font-semibold mb-4">Historical Data</h2>
          <HistoryPanel />
        </div>
      </main>
    </div>
  );
}

export default App;

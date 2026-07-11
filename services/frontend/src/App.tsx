import { VideoFeed } from './components/VideoFeed';
import { StatusIndicator } from './components/StatusIndicator';
import { useStreamStatus } from './hooks/useStreamStatus';

function App() {
  const connected = useStreamStatus();

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
        <div className="grid grid-cols-1 lg:grid-cols-3 gap-6">
          {/* Video Feed - takes 2/3 width on large screens */}
          <div className="lg:col-span-2">
            <h2 className="text-sm font-medium text-gray-400 uppercase tracking-wider mb-3">
              Live Feed
            </h2>
            <VideoFeed connected={connected} />
          </div>

          {/* Right panel - placeholder for Phase 2 */}
          <div>
            <h2 className="text-sm font-medium text-gray-400 uppercase tracking-wider mb-3">
              Recent Detections
            </h2>
            <div className="bg-gray-900 rounded-lg border border-gray-700 p-4 h-96 flex items-center justify-center">
              <p className="text-gray-600 text-sm">
                Detection feed coming in Phase 2
              </p>
            </div>
          </div>
        </div>

        {/* Bottom panels - placeholder for Phase 2 */}
        <div className="grid grid-cols-1 md:grid-cols-2 gap-6 mt-6">
          <div>
            <h2 className="text-sm font-medium text-gray-400 uppercase tracking-wider mb-3">
              Class Distribution
            </h2>
            <div className="bg-gray-900 rounded-lg border border-gray-700 p-4 h-64 flex items-center justify-center">
              <p className="text-gray-600 text-sm">Charts coming in Phase 2</p>
            </div>
          </div>
          <div>
            <h2 className="text-sm font-medium text-gray-400 uppercase tracking-wider mb-3">
              Detections Over Time
            </h2>
            <div className="bg-gray-900 rounded-lg border border-gray-700 p-4 h-64 flex items-center justify-center">
              <p className="text-gray-600 text-sm">Charts coming in Phase 2</p>
            </div>
          </div>
        </div>
      </main>
    </div>
  );
}

export default App;

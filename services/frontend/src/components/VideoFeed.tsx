import { STREAM_URL } from '../config';

interface VideoFeedProps {
  connected: boolean;
}

export function VideoFeed({ connected }: VideoFeedProps) {
  return (
    <div className="relative bg-gray-900 rounded-lg overflow-hidden border border-gray-700">
      {connected ? (
        <img
          src={STREAM_URL}
          alt="Live detection feed"
          className="w-full h-auto"
        />
      ) : (
        <div className="flex items-center justify-center h-96 text-gray-500">
          <div className="text-center">
            <div className="text-4xl mb-4">No Signal</div>
            <p className="text-sm">Waiting for capture-inference stream on port 8081</p>
          </div>
        </div>
      )}
    </div>
  );
}

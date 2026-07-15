import { useState, useEffect, useCallback } from 'react';

export function useStreamStatus(pollInterval = 3000) {
  const [connected, setConnected] = useState(false);

  const checkStatus = useCallback(async () => {
    try {
      const controller = new AbortController();
      const timeout = setTimeout(() => controller.abort(), 2000);
      const res = await fetch('/stream', {
        method: 'HEAD',
        signal: controller.signal,
      });
      clearTimeout(timeout);
      setConnected(res.ok);
    } catch {
      setConnected(false);
    }
  }, []);

  useEffect(() => {
    // Assume connected on mount — show the img tag immediately
    // and let the check correct it if the stream is actually down
    setConnected(true);

    checkStatus();
    const interval = setInterval(checkStatus, pollInterval);
    return () => clearInterval(interval);
  }, [checkStatus, pollInterval]);

  return connected;
}

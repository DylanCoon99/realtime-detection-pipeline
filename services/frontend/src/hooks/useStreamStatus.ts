import { useState, useEffect, useCallback } from 'react';
import { STREAM_URL } from '../config';

export function useStreamStatus(pollInterval = 3000) {
  const [connected, setConnected] = useState(false);

  const checkStatus = useCallback(() => {
    const img = new Image();
    img.onload = () => setConnected(true);
    img.onerror = () => setConnected(false);
    img.src = STREAM_URL + '?t=' + Date.now();
  }, []);

  useEffect(() => {
    checkStatus();
    const interval = setInterval(checkStatus, pollInterval);
    return () => clearInterval(interval);
  }, [checkStatus, pollInterval]);

  return connected;
}

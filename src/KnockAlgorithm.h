#ifndef KnockAlgorithm_h
#define KnockAlgorithm_h

#include <Arduino.h>

class KnockAlgorithm {
    
    public:
        KnockAlgorithm(int lowThreshold);

        void updateReading(int reading);
        void loop();
        
    
    private:
        int _readingsAboveSmallThresholdSum = 0;
        int _readingsAboveSmallThresholdCount = 0;
        int _reading;
        int _state;
        bool _isAwaitingKnock;
        long _currentKnockBurstTime;
        long _lastKnockBurstTime;
        long _knockIntensity;
        long _transientTime;
        long _endOfTailTime;
        long _signalLength;
        long _silenceStartTime;
        long _silenceEndTime;
        const long _silenceTimeout = 50; // 50 ms for waiting for new values to append to signal
};

#endif
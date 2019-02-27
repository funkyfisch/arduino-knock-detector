#ifndef KnockDetector_h
#define KnockDetector_h

#if defined(ESP8266) || defined(ESP32)
#include <functional>
#define CALLBACK std::function<void(float knockIntensity, int pulseLength)> callback
#else
#define CALLBACK void (*callback)(float knockIntensity, int pulseLength)
#endif


#include <Arduino.h>

#include "KnockPulse.h"

class KnockDetector {
    
    public:
        KnockDetector(int lowThreshold, int noiseThreshold, CALLBACK);
        void loop(int reading);

        KnockDetector& setCallback(CALLBACK);
        
    private:
        CALLBACK;
        int _reading;
        int _state;
        bool _isAwaitingKnockTransient;
        long _currentKnockBurstTime;
        long _lastKnockBurstTime;
        long _silenceStartTime;
        long _silenceEndTime;
        int _lowThreshold;
        int _noiseThreshold;
        bool _mustReadValue = false;
        const long _silenceTimeout = 20 * 1000; // 50 ms for waiting for new values to append to signal

        KnockPulse _knockPulse;
};

#endif
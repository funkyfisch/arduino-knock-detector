#ifndef KnockAlgorithm_h
#define KnockAlgorithm_h

#if defined(ESP8266) || defined(ESP32)
#include <functional>
#define CALLBACK std::function<void(int knockIntensity, int pulseLength)> callback
#else
#define CALLBACK void (*callback)(int knockIntensity, int pulseLength)
#endif


#include <Arduino.h>

class KnockAlgorithm {
    
    public:
        KnockAlgorithm(int lowThreshold);
        void loop(int reading);
        
    private:
        int _reading;
        int _state;
        bool _isAwaitingKnock;
        long _currentKnockBurstTime;
        long _lastKnockBurstTime;
        long _silenceStartTime;
        long _silenceEndTime;
        const long _silenceTimeout = 50; // 50 ms for waiting for new values to append to signal

        KnockPulse _knockPulse = NULL;
};

#endif
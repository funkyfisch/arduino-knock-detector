#ifndef KnockPulse_h
#define KnockPulse_h

class KnockPulse {
    public:
        KnockPulse(int value, long time);
        double getKnockIntensity();
        
        int transientValue;
        long pulseStartTime;
        long pulseEndTime;
        int valuesCount;
        int valuesSum;
};

#endif
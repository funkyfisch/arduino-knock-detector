#ifndef KnockPulse_h
#define KnockPulse_h

class KnockPulse {
    public:
        KnockPulse();
        KnockPulse(int value, long time);
        double getKnockIntensity();
        long getKnockLength();
        
        int mTransientValue = 0;
        long mPulseStartTime = 0;
        long mPulseEndTime = 0;
        int mValuesCount = 0;
        int mValuesSum = 0;
};

#endif
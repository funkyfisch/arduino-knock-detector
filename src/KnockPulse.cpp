#include "KnockPulse.h"

KnockPulse::KnockPulse() {
    mTransientValue = 0;
    mPulseStartTime = 0;

    mTransientValue = 0;
    mPulseStartTime = 0;
    mPulseEndTime = 0;
    mValuesCount = 0;
    mValuesSum = 0;
}

KnockPulse::KnockPulse(int transientValue, long pulseStartTime) {
    mTransientValue = transientValue;
    mPulseStartTime = pulseStartTime;

    mTransientValue = 0;
    mPulseStartTime = 0;
    mPulseEndTime = 0;
    mValuesCount = 0;
    mValuesSum = 0;
}

double KnockPulse::getKnockIntensity() {
    return (double) mValuesSum / (double) mValuesCount;
}

long KnockPulse::getKnockLength() {
    return mPulseEndTime - mPulseStartTime;
}
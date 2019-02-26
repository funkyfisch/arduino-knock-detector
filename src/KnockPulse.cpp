#include "KnockPulse.h"

KnockPulse::KnockPulse(int transientValue, long pulseStartTime) {
    transientValue = transientValue;
    pulseStartTime = pulseStartTime;
}

KnockPulse::getKnockIntensity() {
    return (double) valuesSum / (double) valuesCount;
}
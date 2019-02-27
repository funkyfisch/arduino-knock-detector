#include "KnockAlgorithm.h"

// TODO: get rid of arduino platform dependencies
// TODO: write unit tests
// TODO: decouple algorithm from the entrypoint module
// TODO: define readable state machines

// TODO: exchange the "silence timeout" algorithm to a logic check
// where a value followed by a bigger value is treated as a separate signal
// TODO: handle micros() reset to 0
// TODO: proper reassignment of _knockPulse object instead of having singleton
// TODO: provide a calculation of the peak amplitude based on mVolts
// TODO: use JLED library as a golden sample for ci and config
// TODO: add entry for platform.io

KnockAlgorithm::KnockAlgorithm(int lowThreshold, int noiseThreshold, CALLBACK) {
    _reading = 0;
    _isAwaitingKnockTransient = true;
    _lowThreshold = lowThreshold;
    _noiseThreshold = noiseThreshold;

    _silenceStartTime = _silenceEndTime = micros();

    _currentKnockBurstTime = _lastKnockBurstTime = micros();

    setCallback(callback);
}

KnockAlgorithm& KnockAlgorithm::setCallback(CALLBACK) {
    this->callback = callback;
    return *this;
}

// This needs to be constantly called with a reading that is provided by analog input
// at the front end script
void KnockAlgorithm::loop(int reading) {
    _reading = reading;
    // Update the silence timer so that we timeout if needed
    _silenceEndTime = micros();
    // filter out noisy inputs
    if (_reading < _noiseThreshold) {
        _mustReadValue = false;
    }
    else {
        _mustReadValue = true;
    }

    if (_isAwaitingKnockTransient && _mustReadValue == true && _reading < _lowThreshold) {
        
        // Listen for suitable transient of knock. If we detect a transient, update
        // the state to "READING_TAIL" and register the start time of the signal( timestamp of transient)

        // Create new KnockPulse object
        // set the transient(peak) amplitude
        // set transient time and prepare for !_isAwaitingKnockTransient state
        long transientTime = micros();
        _knockPulse.mTransientValue = _reading;
        _knockPulse.mPulseStartTime = micros();
        _isAwaitingKnockTransient = false;
        // Reset the silence timeout
        _silenceEndTime = _silenceStartTime = micros();
    }
    // This will execute if we are in the state of reading the tail of the signal (post-transient)
    // If we are in a READING_PULSE state, we need to define a timeout
    if (!_isAwaitingKnockTransient) {
        
        if (_silenceEndTime - _silenceStartTime <= _silenceTimeout) {
            // reset silence timeout to keep reading values
            if (_mustReadValue == false) return; 
            _silenceStartTime = micros();
            
            // append new reading
            _knockPulse.mValuesCount++;
            _knockPulse.mValuesSum += _reading;
            // update end of tail time
            _knockPulse.mPulseEndTime = micros();

        }
        // if no proper reading is registered, refresh the timer. 
        // if the timer exceeds the timeout
        // we are no longer reading the signal and need to go back to isAwaitingKnock state 
        else {
            // Currently we use an average, but thats not optimal, since the signal
            // needs to be measured in a different way (maybe one number to show the peak transient,
            // and an approximation of time of the total wave)

            callback(_knockPulse.getKnockIntensity(), _knockPulse.getKnockLength());
            //prepare for the isAwaitingKnock state
            _isAwaitingKnockTransient = true;
        }
        
    }
}
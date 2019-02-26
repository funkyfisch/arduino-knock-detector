#include "KnockAlgorithm.h"

// TODO: get rid of arduino platform dependencies
// TODO: write unit tests
// TODO: decouple algorithm from the entrypoint module
// TODO: define readable state machines

// TODO: exchange the "silence timeout" algorithm to a logic check
// where a value followed by a bigger value is treated as a separate signal
// TODO: handle micros() reset to 0

KnockAlgorithm::KnockAlgorithm(int lowThreshold, int noiseThreshold, void *Callback) {
    _reading = 0;
    _isAwaitingKnock = true;

    _silenceStartTime = _silenceEndTime = micros();

    _currentKnockBurstTime = _lastKnockBurstTime = micros();
}

// This needs to be constantly called with a reading that is provided by analog input
// at the front end script
void KnockAlgorithm::loop(int _reading) {
    // filter out noisy inputs
    if (_reading <= _noiseThreshold) return;

    if (_isAwaitingKnockTransient) {
        // Listen for suitable transient of knock. If we detect a transient, update
        // the state to "READING_TAIL" and register the start time of the signal( timestamp of transient)
        if (_reading <= 8 ) return;
        // Create new KnockPulse object
        // set the transient(peak) amplitude
        // set transient time and prepare for !_isAwaitingKnockTransient state
        _knockPulse = KnockPulse(_reading, micros());
        _isAwaitingKnockTransient = false;
        // Reset the silence timeout
        _silenceEndTime = _silenceStartTime = micros();
    }
    // This will execute if we are in the state of reading the tail of the signal (post-transient)
    // If we are in a READING_PULSE state, we need to define a timeout
    if (!_isAwaitingKnockTransient ) {

        if (_silenceEndTime - _silenceStartTime <= _silenceTimeout) {
            // reset silence timeout to keep reading values
            _silenceStartTime = micros();
            // append new reading
            _knockPulse.valuesCount++;
            _knockPulse.valuesSum += _reading;
            // update end of tail time
            _knockPulse.pulseEndTime = micros();
        }
        // if no proper reading is registered, refresh the timer. 
        // if the timer exceeds the timeout
        // we are no longer reading the signal and need to go back to isAwaitingKnock state 
        else {
            // Currently we use an average, but thats not optimal, since the signal
            // needs to be measured in a different way (maybe one number to show the peak transient,
            // and an approximation of time of the total wave)

            // TODO: use callback here to return a detected signal, with its length and intensity
            Serial.print("Length of signal: ");Serial.print(_knockPulse.pulseEndTime - _knockPulse.pulseStartTime);Serial.println(" us");
            Serial.print("Knock intensity: ");Serial.println(_knockPulse.getKnockIntensity());

            //prepare for the isAwaitingKnock state
            _isAwaitingKnockTransient = true;
        }

        // Update the silence timer so that we timeout if needed
        _silenceEndTime = micros();
    }
}
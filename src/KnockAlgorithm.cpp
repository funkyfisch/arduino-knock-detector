#include "KnockAlgorithm.h"

// TODO: get rid of arduino platform dependencies
// TODO: write unit tests
// TODO: decouple algorithm from the entrypoint module
// TODO: define readable state machines

KnockAlgorithm::KnockAlgorithm(int lowThreshold) {
    _reading = 0;
    _isAwaitingKnock = true;
    _currentKnockBurstTime = millis();
    _lastKnockBurstTime = millis();

    _transientTime = 0;
    _endOfTailTime = 0;

    _silenceStartTime = 0;
    _silenceEndTime = 0;


    _signalLength = 0;

    _readingsAboveSmallThresholdSum = 0;
    _readingsAboveSmallThresholdCount = 0;


    _knockIntensity = 0.0;
}

void KnockAlgorithm::updateReading(int reading) {
    _reading = reading;
}

// This needs to be constantly called right after the updateReading() is called
// by the main module with the updated reading.
void KnockAlgorithm::loop() {
  
    if (_isAwaitingKnock) {
        // Listen for suitable transient of knock. If we detect a transient, update
        // the state to "READING_TAIL" and register the start time of the signal( timestamp of transient)
        if (_reading > 8 ) {
            // reset transient time and prepare for !_isAwaitingKnock state
            _transientTime = millis();
            _isAwaitingKnock = false;
            // Knock burst start
        }
    }

    // This will execute if we are in the state of reading the tail of the signal (post-transient)
    // If we are in a READING_TAIL state, we need to define a timeout
    if (!_isAwaitingKnock ) {
        // filter out noisy inputs
        if (_reading > 5) {
            // append new reading
            _readingsAboveSmallThresholdCount++;
            _readingsAboveSmallThresholdSum += _reading;
            // reset timeout to keep reading values
            _silenceStartTime = millis();
            // update end of tail time
            _endOfTailTime = millis();
        }
        // if no proper reading is registered, refresh the timer. 
        //if the timer exceeds the timeout
        // we are no longer reading the signal and need to go back to isAwaitingKnock state 
        else if (_silenceEndTime - _silenceStartTime > _silenceTimeout) {
            // Currently we use an average, but thats not optimal, since the signal
            // needs to be measured in a different way (maybe one number to show the peak transient,
            // and an approximation of time of the total wave)
            double avg = (double)_readingsAboveSmallThresholdSum / (double)_readingsAboveSmallThresholdCount;
            _knockIntensity = avg;

            // TODO: use callback here to return a detected signal, with its length and intensity
            Serial.print("Length of signal: ");Serial.print(_endOfTailTime - _transientTime);Serial.println(" ms");
            Serial.print("Knock intensity: ");Serial.println(_knockIntensity);
            // reset counters and prepare for the isAwaitingKnock state
            _readingsAboveSmallThresholdCount = 0;
            _readingsAboveSmallThresholdSum = 0;
            _silenceStartTime = 0;
            _endOfTailTime = 0;
            _transientTime = 0;
            _isAwaitingKnock = true;
        }

        // Update the silence timer so that we timeout if needed
        _silenceEndTime = millis();
    }
}
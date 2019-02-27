# Arduino Knock Detection Algorithm

### A simple library for getting a single, filtered value for a "knock" event

## Work in Progress - Watch the repo for follow-ups or contribute
### Check source files for TODOs

This library provides a <b>non-blocking</b> way of processing the input from an analog input source, from a piezo/knock/vibration sensor. 

Inspired by "knock" projects, and my work on the [Door Knock Notifier](http://www.github.com/funkyfisch/door-knock-notifier), I decided to modularize this logic behind it into a library.

## Usage

### API
Connect your knock sensor module on Analog Pin 0, according to instructions [here](https://programmingelectronics.com/how-to-make-a-secret-knock-detector-to-trigger-anything-with-only-an-arduino-and-a-few-cheap-components/) or [here](https://www.mousa-simple-projects.com/2015/10/knock-sensor-using-arduino-and-piezo.html)

Declare a callback method that will execute everytime a knock is detected.
```cpp
void callback(float, long);
```
Declare the two parameters <b>lowThreshold</b> and <b>noiseThreshold</b>
```cpp
int lowThreshold = 10; // The lowest value a transient is permitted to have
int noiseThreshold = 3; // Used to filter out analogRead noise
```
Create an instance of <b>KnockAlgorithm</b>
```cpp
KnockAlgorithm knockAlgorithm(lowThreshold, noiseThreshold, callback);
```
<b>loop()</b> Function:
```cpp
void loop() {
    knockAlgorithm.loop(analogRead(A0)) // Or use another analog pin
}
```

### Example sketch

```cpp
#include "src/KnockAlgorithm.h"

const int sensorInputPin = A0;

void callback(float, long) {}
KnockAlgorithm knockAlgorithm(20, 5, callback);

void callback(float knockIntensity, long pulseLength) {
  Serial.println("Knock detected!");
  Serial.print("Knock intensity: ");Serial.println(knockIntensity);
  Serial.print("Knock Pulse length in microseconds: ");Serial.println(pulseLength);
}

void setup() {
  Serial.begin(9600);
}

void loop() {
    knockAlgorithm.loop(analogRead(piezoInputPin));
}
```

## Detailed Explanation
By using states and timers, this algorithm will detect a transient (peak) input that will trigger a listener which will record the entire signal until there is an amount of silence. After a specified amount of silence, the listener times out, and the algorithm returns to its idle state, waiting for a new transient.

The advantages of this method is that instead of having raw values thrown out by the <i> analogRead </i> method, the algorithm returns a single value representing the entirety of a full knock event.

Simply reading the raw values does not provide context or continuity (e.g. we do not know whether a random set of values refers to the same physical knock or are parts of different signals).
/*
 Basic KnockDetection Example

 This sketch demonstrates the capabilities of the KnockDetection Library.

 By attaching a piezoelectric disc to Analog 0 and using the built-in LED,
 you can try knocking or tapping on the surface the disc is attached to
 and the LED will instantly toggle ON or OFF.
*/

#include <KnockDetector.h>

const int piezoInputPin = A0;
int piezoInputSignal = 0;
bool ledOn = false;

void callback(float knockIntensity, long pulseLength) {
  ledOn = !ledOn;
  digitalWrite(BUILTIN_LED, ledOn);
  Serial.println("Knock detected!");
  Serial.print("Knock intensity: ");Serial.println(knockIntensity);
  Serial.print("Knock Pulse length: ");Serial.println(pulseLength);
}

KnockDetector knockDetector(20, 5, callback);

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
}

void loop() {
  knockDetector.loop(analogRead(piezoInputPin));
}
# Arduino Knock Detection Algorithm

### A simple library for getting a single, filtered value for a "knock" event

## Work in Progress - Watch the repo for follow-ups or contribute
### Check source files for TODOs

This library provides a <b>non-blocking</b> way of processing the input from an analog input source, from a piezo/knock/vibration sensor. 

Inspired by "knock" projects, and my work on the [Door Knock Notifier](http://www.github.com/funkyfisch/door-knock-notifier), I decided to modularize this logic behind it into a library.

By using states and timers, this algorithm will detect a transient (peak) input that will trigger a listener which will record the entire signal until there is an amount of silence. After a specified amount of silence, the listener times out, and the algorithm returns to its idle state, waiting for a new transient.

The advantages of this method is that instead of having raw values thrown out by the <i> analogRead </i> method, the algorithm returns a single value representing the entirety of a full knock event.

Simply reading the raw values does not provide context or continuity (e.g. we do not know whether a random set of values refers to the same physical knock or are parts of different signals).
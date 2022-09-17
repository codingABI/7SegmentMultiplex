# 7SegmentMultiplex
Demonstration of segment multiplexing for a 7-segment display with an Arduino Uno/Nano.

This technic could be used to use a 7-segment display with a common anode or cathode (for example CL4041AH)
- directly by an Arduino Uno/Nano and
- without a special display controller and 
- without too much current on microcontroller I/O-pins 
   (Max. 40mA per I/O-pin would be allowed for an ATMega328 and 200mA for all I/O-pins in sum.
   We will use only ~11mA per I/O-pin and in sum 11mA*"Number of digits" due segment muliplexing, for example 44mA for a 4 digit display)
- with only one current limiting resistors per digit and
- without bightness flickering depending on how many segments of digit are used

Disadvantages:
- We use use only 1/8 of the maximum possible bightness of the display
- We have to update the display every ~2ms to avoid flickering (for example by timer2)

![captured display sequence](/assets/images/Capture.gif) 

Simulation https://wokwi.com/projects/342698937471205971

Video https://www.youtube.com/watch?

## Used 7-segment display
Model: 4041AH

Type: Common cathode

### Segments
![segments](/assets/images/segments.png)

### Pinout (Top view)

![View from top](/assets/images/viewFromTop.png)

| Pin | 12 | 11 | 10 | 9 | 8 | 7 |
| --- | --- | --- | --- | --- | --- | --- |
| Usage | DIG1 | A | F | DIG2 | DIG3 | B |

| Pin | 1  | 2  | 3 | 4 | 5 | 6 |
| --- | --- | --- | --- | --- | --- | --- |
| Usage | E | D | DP | C | G | DIG4 |

## Schema
![schema](/assets/images/schema.png)

## Breadboard
![Breadboard](/assets/images/Breadboard.jpg) 

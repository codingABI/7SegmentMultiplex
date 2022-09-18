# 7SegmentMultiplexing
Demonstration of segment multiplexing for a 7-segment display with an Arduino Uno/Nano.

Principle:
- Only one segment per time is enabled
- All digits uses the same segment per time
- Every digit, where the current segment should be shown, is enabled 
- On every display update the next segment will be enabled

This technic could be used to drive a 7-segment display with a common anode or cathode (for example 4041AH):
- directly by an Arduino Uno/Nano and
- without a special display controller and 
- without too much current on microcontroller I/O-pins 
   (Max. 40mA per I/O-pin would be allowed for an ATmega328P and 200mA for all I/O-pins in sum.
   We will use only ~11mA per I/O-pin and in sum 11mA*"Number of digits" due segment muliplexing, for example 44mA for a 4 digit display)
- with only one current limiting resistors per digit and
- without bightness flickering depending on how many segments of digit are used

Disadvantages:
- We use use only 1/8 of the maximum possible bightness of the display
- We have to update the display every ~2ms to avoid flickering (for example by timer2)

![captured display sequence](/assets/images/Capture.gif) 

Simulation https://wokwi.com/projects/342698937471205971

Video https://youtu.be/b8dgzbctinw

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


DIG1 = Common cathode for digit on left side
...
DIG4 = Common cathode for digit on right side

## Schema
![schema](/assets/images/schema.png)

## Breadboard
![Breadboard](/assets/images/Breadboard.jpg) 

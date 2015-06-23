# ShiftDisplay
Arduino library for driving multiple digit 7-segment displays using shift registers

- Only 3 pins used on Arduino
- Use displays with any quantity of digits
- Common cathode and common anode compatible
- Show numbers and text
- Concatenate multiple displays as one

![breadboard](https://raw.githubusercontent.com/Pyntoo/ShiftDisplay/master/extra/photo.jpg)

## Hardware
- 1x (or more) Multiple-Digit 7-Segment LED Display (common anode or common cathode)
- 2x (or more) 74HC595 Shift Register (other 8-bit shift registers should work)
- 8x 220 Ohm Resistor

## Install
1. Download library https://github.com/Pyntoo/ShiftDisplay/archive/master.zip
2. Extract to `<arduino-sketch-folder>/libraries/` folder.
3. Rename extracted folder `ShiftDisplay-master` to `ShiftDisplay`.
4. Restart the Arduino IDE.

## Wiring
1. Connect Arduino voltage to shift registers:
![wiring arduino voltage to shift register](https://raw.githubusercontent.com/Pyntoo/ShiftDisplay/master/extra/arduino1_to_shift.png)

2. Connect Arduino outputs to shift registers:
![wiring arduino output to shift register](https://raw.githubusercontent.com/Pyntoo/ShiftDisplay/master/extra/arduino2_to_shift.png)

3. Connect shift registers to display:
![wiring shift register to display](https://raw.githubusercontent.com/Pyntoo/ShiftDisplay/master/extra/shift_to_display.png)

## Reference
**ShiftDisplay(int latchPin, int clockPin, int dataPin, bool commonCathode, int displaySize)**

Creates display object.
latchPin, clockPin and dataPin are the shift register pins connected to the Arduino.
commonCathode is a boolean for the led type.
displaySize is the quantity of digits the display has.

-
**void print(int value, int time)**

Show an integer value in the display, for the given time in milliseconds,
with specified animation and alignment constants.

-
**void print(float value, int decimalPlaces, int time)**

Show a float value, rounded to specified number of decimal places, for the
given time in milliseconds, with specified animation and alignment constants.

-
**void print(String text, int time)**

Show text in the display, for the given time in milliseconds,
with specified animation and alignment constants.
Accepted characters are A-Z, a-z, 0-9, -, space.

-
**CONSTANTS**
ANIMATION_NONE
ANIMATION_SCROLL
ANIMATION_FADEIN
ANIMATION_FADEOUT

ALIGNMENT_LEFT
ALIGNMENT_RIGHT
ALIGNMENT_CENTER


## Example
A 3 digit common cathode display, with latch, clock and data pins connected to
6, 7 and 5 respectively.

```c
#include <ShiftDisplay.h>

ShiftDisplay disp(6, 7, 5, true, 3);

void setup() {
}

void loop() {
  // disp.printMenu('t', 9, 2000);
  // disp.printMenu('h', 64, 2000);

  disp.print("  c", 500, ANIMATION_NONE, ALIGNMENT_LEFT);
  disp.print(" cl", 500, ANIMATION_NONE, ALIGNMENT_LEFT);
  disp.print("cle", 500, ANIMATION_NONE, ALIGNMENT_LEFT);
  disp.print("lea", 500, ANIMATION_NONE, ALIGNMENT_LEFT);
  disp.print("ear", 500, ANIMATION_NONE, ALIGNMENT_LEFT);
  disp.print("ar", 500, ANIMATION_NONE, ALIGNMENT_LEFT);
  disp.print("r", 500, ANIMATION_NONE, ALIGNMENT_LEFT);
}
```

```c
#include <ShiftDisplay.h>

ShiftDisplay disp(6, 7, 5, true, 3);

void setup() {
  for (int i = 42; i > 0; i--)
    disp.print(i, 400, ANIMATION_NONE, ALIGNMENT_RIGHT);
}

void loop() {
  disp.print("hey", 1, ANIMATION_NONE, ALIGNMENT_RIGHT);
}
```

## Changelog
- 1.0.0 (26/05/2015)
  - Initial release
- 2.0.0 (23/06/2015)
  - Rewrite library
  - Default (full) and Mini (lightweigth) versions
  - Control over alignment

## TODO
- [ ] Improvement: examples comments
- [ ] Feature: ALIGNMENT_CENTER
- [ ] Feature: ANIMATION_SCROLL
- [ ] Feature: ANIMATION_FADEIN
- [ ] Feature: ANIMATION_FADEOUT

---

https://github.com/Pyntoo/ShiftDisplay/

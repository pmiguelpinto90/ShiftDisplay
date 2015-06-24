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
###Constructors
**ShiftDisplay(int latchPin, int clockPin, int dataPin, bool commonCathode, int displaySize)**

Creates default ShiftDisplay object using 3 pins, fade animations disabled.
latchPin, clockPin and dataPin are the shift register inputs connected to
the Arduino digital outputs.
commonCathode is true if the led type is common cathode, false if it's common anode.
displaySize is the quantity of digits of all displays together.

-
**ShiftDisplay(int latchPin, int clockPin, int dataPin,int outputEnablePin, bool commonCathode, int displaySize) {**

Creates advanced ShiftDisplay object using 4 pins, fade animations enabled.
latchPin, clockPin, dataPin and outputEnablePin are the shift register inputs
connected to the Arduino digital outputs. outputEnablePin must be connected to a
PWM pin.
commonCathode is true if the led type is common cathode, false if it's common anode.
displaySize is the quantity of digits of all displays together.

-
**ShiftDisplayMini(int latchPin, int clockPin, int dataPin, bool commonCathode, int displaySize)**
Creates lightweight ShiftDisplay object using 3 pins.
You must `#include <ShiftDisplayMini.h>` instead of `<ShiftDisplay.h>` to use this.
latchPin, clockPin and dataPin are the shift register inputs connected to
the Arduino digital outputs.
commonCathode is true if the led type is common cathode, false if it's common anode.
displaySize is the quantity of digits of all displays together, a maximum of 8.

###Functions (ShiftDisplay)
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

###Functions (ShiftDisplayMini)
**void print(int value, int time)**
Show an integer value, right aligned in the display,
for the given time in milliseconds.

-
**void print(float value, int decimalPlaces, int time)**
Show a float value, rounded to specified decimal places,
right aligned in the display, for the given time in milliseconds.

-
**void print(String text, int time)**
Show text, left aligned in the display, for the given time in milliseconds.
Accepted characters are A-Z, a-z, 0-9, -, space.

###Constants
- ANIMATION_NONE
- ANIMATION_SCROLL
- ANIMATION_FADEIN
- ANIMATION_FADEOUT
- ALIGNMENT_LEFT
- ALIGNMENT_RIGHT
- ALIGNMENT_CENTER


## Example
```c
#include <ShiftDisplay.h>

// A 4 digit common anode display. With latch, clock and data connected
// to 6, 7 and 5 pins respectively.
ShiftDisplay disp(6, 7, 5, false, 4);

void setup() {
}

void loop() {
  // disp.printMenu('t', 9, 2000);
  // disp.printMenu('h', 64, 2000);

  disp.print("c", 500, ANIMATION_NONE, ALIGNMENT_RIGHT);
  disp.print("cl", 500, ANIMATION_NONE, ALIGNMENT_RIGHT);
  disp.print("cle", 500, ANIMATION_NONE, ALIGNMENT_RIGHT);
  disp.print("clea", 500, ANIMATION_NONE, ALIGNMENT_RIGHT);
  disp.print("lear", 500, ANIMATION_NONE, ALIGNMENT_LEFT);
  disp.print("ear", 500, ANIMATION_NONE, ALIGNMENT_LEFT);
  disp.print("ar", 500, ANIMATION_NONE, ALIGNMENT_LEFT);
  disp.print("r", 500, ANIMATION_NONE, ALIGNMENT_LEFT);
  disp.print("", 500, ANIMATION_NONE, ALIGNMENT_LEFT);
}
```

```c
#include <ShiftDisplayMini.h>

// A 3 digit common cathode display. With latch, clock and data connected
// to 6, 7 and 5 pins respectively.
ShiftDisplayMini disp(6, 7, 5, true, 3);

void setup() {
  for (int i = 42; i > 0; i--)
    disp.print(i, 400);
}

void loop() {
  disp.print("pum", 1);
}
```

## Changelog
- 1.0.0 (26/05/2015)
  - Initial release
- 2.0.0 (23/06/2015)
  - Rewrite library
  - Full and lightweight versions
  - Control over alignment

## TODO
- [ ] Improvement: examples comments
- [ ] Feature: ALIGNMENT_CENTER
- [ ] Feature: ANIMATION_SCROLL
- [ ] Feature: ANIMATION_FADEIN
- [ ] Feature: ANIMATION_FADEOUT

---

https://github.com/Pyntoo/ShiftDisplay/

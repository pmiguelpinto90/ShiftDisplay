# BUILDING README

# ShiftDisplay
Arduino library for driving multiple digit 7-segment displays using shift registers

- Only 3 pins used on Arduino
- Displays with any quantity of digits
- Common cathode and common anode compatible
- Show numbers or text
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
![wiring arduino voltage to shift register](https://raw.githubusercontent.com/Pyntoo/ShiftDisplay/master/extra/arduino1_to_shift.png)
-
![wiring arduino output to shift register](https://raw.githubusercontent.com/Pyntoo/ShiftDisplay/master/extra/arduino2_to_shift.png)

![wiring shift register to display](https://raw.githubusercontent.com/Pyntoo/ShiftDisplay/master/extra/shift_to_display.png)

## Reference
**ShiftDisplay(int latchPin, int clkPin, int dataPin, bool commonCathode, int nDigits)**

Creates object display.
latchPin, clkPin and dataPin are the shift register pins connected to the Arduino.
commonCathode is a boolean for the led type.
nDigits are the number of digits of the led.

-
**bool print(int value, int milliseconds)**

Displays integer value, right aligned in display, for the given milliseconds.
Returns true if displayed whole number.

-
**bool print(float value, int nDecimalPlaces, int milliseconds)**

Displays float value, right aligned in display, rounded to nDecimalPlaces,
for the given milliseconds.
Returns true if displayed whole number.

-
**bool print(String text, int milliseconds)**

Displays text, left aligned in display, for the given milliseconds.
Accepted characters for string are A-Z, a-z, 0-9, -, space.
Returns true if displayed whole string.

-
**bool printMenu(char c, int value, int milliseconds)**

Displays character c left aligned in display, and integer value right aligned
in display, for the given milliseconds.
Returns true if displayed whole value.

## Example
A 3 digit common cathode display, with latch, clock and data pins connected to
6, 7 and 5 respectively.

```c
#include <ShiftDisplay.h>

ShiftDisplay disp(6, 7, 5, true, 3);

void setup() {
}

void loop() {
  disp.printMenu('t', 9, 2000);
  disp.printMenu('h', 64, 2000);

  disp.print("  c", 500);
  disp.print(" cl", 500);
  disp.print("cle", 500);
  disp.print("lea", 500);
  disp.print("ear", 500);
  disp.print("ar", 500);
  disp.print("r", 500);
}
```

```c
#include <ShiftDisplay.h>

ShiftDisplay disp(6, 7, 5, true, 3);

void setup() {
  for (int i = 42; i > 0; i--)
    disp.print(i, 400);
}

void loop() {
  disp.print("hey", 1);
}
```

## TODO
- [x] Function: printMenu
- [ ] Improve: examples comments
- [ ] Bug: overflow when rounding float
- [ ] Function: scroll
- [ ] Function: fadeIn
- [ ] Function: fadeOut

# ShiftDisplay

Arduino library for driving multiple-digit 7-segment LED displays using 74HC595 shift registers

- Show numbers and text
- Concatenate multiple displays as one, for a maximum of 8 digits
- Compatible with common cathode and common anode
- Only 3 pins used on Arduino

![breadboard](https://raw.githubusercontent.com/MiguelPynto/ShiftDisplay/master/extra/extra.jpg)

________________________________________________________________________________
## Hardware

- 1x (or more) 7-segment LED display
- 2x 74HC595 shift register
- 8x (per display) 220 Ohm resistor

________________________________________________________________________________
## Install

1. Download library from https://github.com/MiguelPynto/ShiftDisplay/archive/master.zip
2. Extract to `Documents/Arduino/libraries` folder.
3. Rename extracted folder `ShiftDisplay-master` to `ShiftDisplay`.
4. Restart the Arduino IDE.

________________________________________________________________________________
## Wiring

1. Connect Arduino to shift registers:
![input](https://raw.githubusercontent.com/MiguelPynto/ShiftDisplay/master/extra/input.png)

2. Connect shift registers to display(s):
![output](https://raw.githubusercontent.com/MiguelPynto/ShiftDisplay/master/extra/output.png)

________________________________________________________________________________
## Reference

### Constructors

* __ShiftDisplay()__
  * ShiftDisplay led(latchPin, clockPin, dataPin, displayType, displayLength)

  Creates ShiftDisplay object, initializes the library with the interface pins, and sets up with the display properties.

  `led`: is a variable of type ShiftDisplay.

  `latchPin`, `clockPin`, `dataPin`: are the number of the Arduino digital pins connected to the shift registers latch, clock and data pins respectively.

  `displayType`: is a constant `COMMON_CATHODE` or `COMMON_ANODE`, depending on the type of your display(s).

  `displayLength`: is the quantity of digits on all displays combined.

### Functions

* __set()__
  * led.set(value)
  * led.set(value, alignment)
  * led.set(value, decimalPlaces)
  * led.set(value, decimalPlaces, alignment)

  Saves a value to buffer for printing latter.

  `led`: is a variable of type ShiftDisplay.

  `value`: is the value to save;
  can be of type int, long, float, double, char, char array or String object;
  for text, valid characters are A-Z, a-z, 0-9, -, space.

  `alignment`: is the alignment of the value on the display;
  optional, constant `ALIGN_LEFT`, `ALIGN_RIGHT` or `ALIGN_CENTER`;
  if not defined, the default is ALIGN_RIGHT for numbers, ALIGN_LEFT for strings, and ALIGN_CENTER for characters;
  if the value is a float or double number, decimalPlaces has to be defined before alignment.

  `decimalPlaces`: is the number of digits following the decimal point;
  optional and only available if the value is a float or double number;
  if not defined, the default is 2.

* __show()__
  * led.show()
  * led.show(time)

  Shows buffer value on the display.

  `led`: is a variable of type ShiftDisplay.

  `time`: is the time in milliseconds for the value to be shown on the display;
  optional; if not defined, the value is shown for a single iteration of each character.

* __print()__
  * led.print(time, value)
  * led.print(time, value, alignment)
  * led.print(time, value, decimalPlaces)
  * led.print(time, value, decimalPlaces, alignment)

  Saves a value to buffer and shows it on the display.

  `led`: is a variable of type ShiftDisplay.

  `time`: is the time in milliseconds for the value to be shown on the display;

  `value`: is the value to save and show.
  can be of type int, long, float, double, char, char array or String object;
  for text, valid characters are A-Z, a-z, 0-9, -, space.

  `alignment`: is the alignment of the value on the display;
  optional, constant `ALIGN_LEFT`, `ALIGN_RIGHT` or `ALIGN_CENTER`;
  if not defined, the default is ALIGN_RIGHT for numbers, ALIGN_LEFT for strings, and ALIGN_CENTER for characters;
  if the value is a float or double number, decimalPlaces has to be defined before alignment.

  `decimalPlaces`: is the number of digits following the decimal point;
  optional and only available if the value is a float or double number;
  if not defined, the default is 2.

### Constants

- ALIGN_LEFT
- ALIGN_RIGHT
- ALIGN_CENTER
- COMMON_ANODE
- COMMON_CATHODE

________________________________________________________________________________
## Example

```c
#include <ShiftDisplay.h>

ShiftDisplay led(6, 7, 5, COMMON_CATHODE, 3);

void setup() {
	for (int i = 3; i > 0; i--)
		led.print(800, i);
	led.set("GO");
}

void loop() {
	led.show();
}
```

________________________________________________________________________________
## Changelog

- 1.0.0 (26/05/2015)
  - Initial release
- 2.0.0 (23/06/2015)
  - Code refactoring
  - Full and lightweight versions
  - Feature: left and right alignment
- 2.1.0 (03/07/2015)
  - Feature: center alignment
- 2.2.0 (14/07/2015)
  - Feature: minimal print function
  - Feature: exit left/right animation
- 3.0.0 (16/11/2016)
  - Code rewriting: performance and behavior improvement
  - Change: function signatures
  - Feature: buffer
  - Documentation: new examples
  - Documentation: updated images

________________________________________________________________________________
## TODO

- [ ] Feature: custom chars
- [ ] Feature: change char in position
- [ ] Feature: punctuation chars
- [ ] Change: function begin with displayLength and displayType
- [ ] Feature: fade
- [ ] Bugfix: time minus pov
- [ ] Bugfix: real number 0.x only shows .x
- [ ] Documentation: rand() to random() in example

________________________________________________________________________________
![breadboard](https://raw.githubusercontent.com/MiguelPynto/ShiftDisplay/master/extra/foobar.jpg)

https://github.com/MiguelPynto/ShiftDisplay

# ShiftDisplay
_by MiguelPynto_

Arduino library for driving multiple-digit 7-segment LED displays using 74HC595 shift registers

- Show numbers and text
- Concatenate multiple displays as one, for a maximum of 8 digits
- Compatible with common cathode and common anode
- Only 3 pins used on Arduino

![breadboard](https://raw.githubusercontent.com/MiguelPynto/ShiftDisplay/master/extras/foobar.jpg)


## Hardware

- 1x (or more) 7-segment LED display
- 2x 74HC595 shift register
- 8x (per display) 220 Ohm resistor


## Install

1. Start the Arduino IDE
1. Open Library Manager (Menu > Sketch > Include Library > Manage Libraries)
2. Search for ShiftDisplay library (Filter your search > shiftdisplay)
3. Select it and click install


## Wiring

1. Connect Arduino to shift registers:
![input](https://raw.githubusercontent.com/MiguelPynto/ShiftDisplay/master/extras/input.png)

2. Connect shift registers to display(s):
![output](https://raw.githubusercontent.com/MiguelPynto/ShiftDisplay/master/extras/output.png)


## Reference

### Constructors

* __ShiftDisplay()__

  * ShiftDisplay display(displayType, displaySize)
  * ShiftDisplay display(displayType, sectionCount, sectionSizes)
  * ShiftDisplay display(latchPin, clockPin, dataPin, displayType, displaySize)
  * ShiftDisplay display(latchPin, clockPin, dataPin, displayType, sectionCount, sectionSizes)

  Create a ShiftDisplay object, initialize the library using the interface pins and display properties.
  A display can be set as a whole, or divided by sections. Sections can be a logical separation (for example hours and minutes on a 4 digit clock display), or a physical separation (for example timer and score on a scoreboard with multiple displays).

  `display`: newly created ShiftDisplay object.

  `displayType`: constant `COMMON_CATHODE` or `COMMON_ANODE`, defining the type of the display.

  `displaySize`: quantity of digits on the display.

  `sectionCount`: quantity of sections on the display.

  `sectionSizes`: array with quantity of digits on each section.

  `latchPin`, `clockPin`, `dataPin`: number of the Arduino digital pins connected to the latch, clock and data pins of the shift registers;
  if not defined, the default pins are 6 for latch, 7 for clock, and 5 for data.

### Functions

* __set()__

  * display.set(value)
  * display.set(value, alignment)
  * display.set(valueReal)
  * display.set(valueReal, alignment)
  * display.set(valueReal, decimalPlaces)
  * display.set(valueReal, decimalPlaces, alignment])
  * display.set(customs)
  * display.set(characters, dots)

  Save a value to buffer for showing latter.

  `display`: object of type ShiftDisplay.

  `value`: value to save;
  can be a number (int, long) or text (char, char array, string object);
  for text, valid characters are 0-9 a-z A-Z -, everything else will be converted to a space.
  
  `valueReal`: float or double number value to save.

  `alignment`: alignment of the value on the display;
  constant `ALIGN_LEFT`, `ALIGN_RIGHT` or `ALIGN_CENTER`;
  if not defined, the default is ALIGN_RIGHT for numbers and ALIGN_LEFT for text.

  `decimalPlaces`: number of digits following the decimal point;
  if not defined, the default is 2.

  `customs`: array of bytes specifying custom characters, each representing a character on the display, from left to right;
  a custom character is encooded as a byte where segment A is MSB and DP is LSB;
  array length must match display size.

  `characters`: array of chars, each representing a character on the display, from left to right;
  valid characters are 0-9 a-z A-Z -, everything else will be converted to a space;
  array length must match display size.

  `dots`: array of bools, each representing a dot on the display, from left to right;
  array length must match display size.

* __setDot()__

  * display.setDot(index, dot)

  Show or hide dot from buffer.

  `display`: object of type ShiftDisplay.

  `index`: position on the display to set the dot, starting at 0 for the leftmost;
  if is invalid, does not have any effect.

  `dot`: true to show, false to hide.

* __setCustom()__

  * display.setCustom(index, custom)

  Replace with a custom character.

  `display`: object of type ShiftDisplay.

  `index`: position on the display to set the character, starting at 0 for the leftmost;
  if is invalid, does not have any effect.

  `custom`: byte TODO

* __setAt()__

  * display.setAt(section, value)
  * display.setAt(section, value, alignment)
  * display.setAt(section, real)
  * display.setAt(section, real, alignment)
  * display.setAt(section, real, decimalPlaces)
  * display.setAt(section, real, decimalPlaces, alignment)
  * display.setAt(section, customs)
  * display.setAt(section, characters, dots)

  Save TODO

  `display`: object of type ShiftDisplay.

  `section`: TODO

  `value`: value to save;
  can be a number (int, long) or text (char, char array, string object);
  for text, valid characters are 0-9 a-z A-Z -, everything else will be converted to a space.
  
  `valueReal`: float or double number value to save.

  `alignment`: alignment of the value on the display;
  constant `ALIGN_LEFT`, `ALIGN_RIGHT` or `ALIGN_CENTER`;
  if not defined, the default is ALIGN_RIGHT for numbers and ALIGN_LEFT for text.

  `decimalPlaces`: number of digits following the decimal point;
  if not defined, the default is 2.

  `customs`: array of bytes specifying custom characters, each representing a character on the display, from left to right;
  a custom character is encooded as a byte where segment A is MSB and DP is LSB;
  array length must match display size.

  `characters`: array of chars, each representing a character on the display, from left to right;
  valid characters are 0-9 a-z A-Z -, everything else will be converted to a space;
  array length must match display size.

  `dots`: array of bools, each representing a dot on the display, from left to right;
  array length must match display size.

* __setDotAt()__

  * display.setDotAt(section, index, dot)

  Show or hide dot TODO

  `display`: object of type ShiftDisplay.

  `section`: TODO

  `index`: position on the display to set the dot, starting at 0 for the leftmost;
  if is invalid, does not have any effect.

  `dot`: true to show, false to hide.

* __setCustomAt()__

  * display.setCustomAt(section, index, custom)

  Replace with a custom character. TODO

  `display`: object of type ShiftDisplay.

  `section`: TODO

  `index`: position on the display to set the character, starting at 0 for the leftmost;
  if is invalid, does not have any effect.

  `custom`: byte TODO

* __show()__

  * display.show()
  * display.show(time)

  Show buffer value on the display.

  `display`: object of type ShiftDisplay.

  `time`: time in milliseconds for the value to be shown on the display;
  has to be >= 10;
  if not defined, the value is shown for a single iteration;
  exact time showing will be an under aproximation.

* __show()__

  * display.show(value, time)
  * display.show(value, time, alignment)
  * display.show(valueReal, time)
  * display.show(valueReal, time, alignment)
  * display.show(valueReal, time, decimalPlaces)
  * display.show(valueReal, time, decimalPlaces, alignment)
  * display.show(customs, time)
  * display.show(characters, dots, time)

  Save a value to buffer and show it on the display.

  `display`: an object of type ShiftDisplay.

  `value`: value to save and show;
  can be a number (int, long) or text (char, char array, String object);
  for text, valid characters are 0-9 a-z A-Z -, everything else will be converted to a space.
  
  `valueReal`: float or double number value to save.

  `time`: time in milliseconds for the value to be shown on the display;
  has to be >= 10;
  exact time showing will be an under aproximation.

  `alignment`: alignment of the value on the display;
  constant `ALIGN_LEFT`, `ALIGN_RIGHT` or `ALIGN_CENTER`;
  if not defined, the default is ALIGN_RIGHT for numbers and ALIGN_LEFT for text.

  `decimalPlaces`: is the number of digits following the decimal point;
  if not defined, the default is 2.

  `customs`: byte array specifying custom characters, each representing a character on the display, from left to right;
  a custom character is encooded as a byte where segment A is MSB and DP is LSB;
  array length must match display size.

  `characters`: char array, each representing a character on the display, from left to right;
  valid characters are 0-9 a-z A-Z -, everything else will be converted to a space;
  array length must match display size.

  `dots`: bool array, each representing a dot on the display, from left to right;
  array length must match display size.

### Constants

- ALIGN_LEFT
- ALIGN_RIGHT
- ALIGN_CENTER
- COMMON_ANODE
- COMMON_CATHODE


## Example

```c
#include <ShiftDisplay.h>

// Arduino pin 6 connected to shift register latch, pin 7 to clock and pin 5 to data
// common cathode display with 3 digits
ShiftDisplay display(6, 7, 5, COMMON_CATHODE, 3);

void setup() {
	for (int i = 3; i > 0; i--) {
		display.show(i, 400, ALIGN_CENTER); // save number and show it for 400ms
		display.setDot(1, true); // add dot to saved number
		display.show(400); // show number with dot for 400ms
	}
	display.set("GO"); // save "GO" to buffer
}

void loop() {
	display.show(); // show "GO" while in loop
}
```


## Changelog

- 3.6.0 (15/9/2017)
  - NEW: sectioned display feature
  - NEW: setAt() function
  - NEW: setDot() function
  - NEW: setDotAt() function
  - NEW: setCustom() function
  - NEW: setCustomAt() function
  - FIX: decimal point would not show in double value at display index 0
  - DOC: reviewed code comments
- 3.5.0 (8/9/2017)
  - Feature: set/show characters array with dot array
  - Feature: set/show custom characters
- 3.4.0 (6/9/2017)
  - Feature: loadDots() function
  - Change: print(time, value) updated to show(value, time)
  - Change: insertPoint() and removePoint() updated to insertDot() and removeDot()
  - Documentation: small fixes
- 3.3.3 (30/3/2017)
  - Change: updated contact details
- 3.3.2 (6/1/2017)
  - Change: faster show() function
  - Bugfix: better non-Arduino support (eg. attiny)
- 3.3.1 (18/12/2016)
  - Bugfix: Countdown example
- 3.3.0 (18/12/2016)
  - Feature: insertPoint() function
  - Feature: removePoint() function
  - Change: removed punctuation characters
- 3.2.3 (26/11/2016)
  - Bugfix: decimal point not showing in common anode displays
- 3.2.2 (25/11/2016)
  - Documentation: website
- 3.2.1 (24/11/2016)
  - Documentation: explain examples
- 3.2.0 (22/11/2016)
  - Bugfix: default alignments
  - Change: float/double function alignment without decimalPlaces obligation
- 3.1.0 (21/11/2016)
  - Bugfix: float/double functions problems with 0.x and -0.x
  - Bugfix: common anode display initiates unclear
  - Feature: new constructor with default pins
  - Feature: punctuation characters
  - Change: small improvements
- 3.0.1 (19/11/2016)
  - Bugfix: float/double functions not working
  - Bugfix: show/print functions exceeding time
- 3.0.0 (17/11/2016)
  - Code rewriting: performance and behavior improvement
  - Change: function signatures
  - Feature: buffer
  - Documentation: new examples
  - Documentation: updated images
- 2.2.0 (14/07/2015)
  - Feature: minimal print function
  - Feature: exit left/right animation
- 2.1.0 (03/07/2015)
  - Feature: center alignment
- 2.0.0 (23/06/2015)
  - Code refactoring
  - Full and lightweight versions
  - Feature: left and right alignment
- 1.0.0 (26/05/2015)
  - Initial release


## TODO

- [x] Sectioned display
- [ ] Improve README
- [ ] Example for sectioned display
- [ ] Example for custom characters
- [ ] INDIVIDUAL_CATHODE and INDIVIDUAL_ANODE display types


## Contacts

miguelpynto@outlook.com

https://miguelpynto.github.io/ShiftDisplay/

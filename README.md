https://miguelpynto.github.io/ShiftDisplay/

# ShiftDisplay
_by MiguelPynto_

Arduino library for driving 7-segment displays using shift registers

- Show numbers and text
- Concatenate multiple displays as one, for a maximum of 8 digits
- Compatible with common cathode and common anode
- Only 3 pins used on Arduino

![breadboard](https://raw.githubusercontent.com/MiguelPynto/ShiftDisplay/master/extras/foobar.jpg)


## Hardware

- 1x (or more) 7-segment LED display
- 2x 74HC595 shift register
- 2x 0.1uF ceramic capacitor
- 8x (per display) 220 Ohm resistor


## Wiring

1. Connect Arduino board to shift registers:
![input](https://raw.githubusercontent.com/MiguelPynto/ShiftDisplay/master/extras/input.png)

2. Connect shift registers to display(s):
![output](https://raw.githubusercontent.com/MiguelPynto/ShiftDisplay/master/extras/output.png)


## Install

1. Launch the Arduino IDE
1. Open the Library Manager by going to menu: Sketch > Include Library > Manage Libraries
2. Search for ShiftDisplay library by typing shiftdisplay in Filter your search
3. Select ShiftDisplay by MiguelPynto and click install


## Example

```c
#include <ShiftDisplay.h>

ShiftDisplay display(COMMON_CATHODE, 3);

void setup() {
	for (int i = 3; i > 0; i--) {
		display.show(i, 400, ALIGN_CENTER); // store number and show it for 400ms
		display.setDot(1, true); // add dot to stored number
		display.show(400); // show number with dot for 400ms
	}
	display.set("GO"); // store "GO"
}

void loop() {
	display.show(); // show stored "GO" while in loop
}
```


## Reference

### Constructors

* __ShiftDisplay()__

  * ShiftDisplay display([latchPin, clockPin, dataPin, ]displayType, displaySize[, displayDrive])
  * ShiftDisplay display([latchPin, clockPin, dataPin, ]displayType, sectionCount, sectionSizes[, displayDrive])

  Create a ShiftDisplay object, initialize the library using the interface pins and display properties.
  A display can be set as a whole, or divided by sections. Sections can be a logical separation (for example hours and minutes on a 4 digit clock display), or a physical separation (for example timer and score on a scoreboard with multiple displays).

  `display`: newly created ShiftDisplay type object.

  `latchPin`, `clockPin`, `dataPin`: number of the Arduino digital pins connected to the latch, clock and data pins of the shift registers;
  if is not specified, the default pins are 6 for latch, 7 for clock, and 5 for data.

  `displayType`: constant `COMMON_ANODE` or `COMMON_CATHODE`, defining the type of the display.

  `displaySize`: quantity of digits on the display.

  `sectionCount`: quantity of sections on the display.

  `sectionSizes`: array of ints initialized with the quantities of digits for each section.

  `displayDrive`: constant `MULTIPLEXED_DRIVE` or `STATIC_DRIVE`, defining the drive algorithm of the display;
  if is not specified, the default is MULTIPLEXED_DRIVE.

### Functions

* __set()__

  * display.set(value[, alignment])
  * display.set(valueReal[, decimalPlaces][, alignment])
  * display.set(customs)
  * display.set(characters, dots)

  Store a value to show next on the display. The entire previous value is cleared.

  `display`: object of type ShiftDisplay.

  `value`: value to set, a number (int, long) or text (char, char array, string object);
  text valid characters are `0-9 a-z A-Z -`, other characters are converted to a space;
  char array must be null-terminated;
  if is too big to fit in the display, it is trimmed according to alignment.
  
  `valueReal`: value to set, a real number (float, double);
  if is too big to fit in the display, it is trimmed according to alignment.

  `alignment`: alignment of the value or valueReal on the display, a constant `ALIGN_LEFT` or `ALIGN_RIGHT` or `ALIGN_CENTER`;
  if is not specified, the default is ALIGN_RIGHT for numbers and ALIGN_LEFT for text.

  `decimalPlaces`: quantity of digits following the decimal point;
  if is not specified, the default is 1.

  `customs`: array of bytes initialized with the custom characters to set, encoded in abcdefgp format, each byte is a character on the display, from left to right;
  array length must match display size.

  `characters`: array of chars initialized with the characters to set, each char is a character on the display, from left to right;
  valid characters are `0-9 a-z A-Z -`, other characters are converted to a space;
  array length must match display size.

  `dots`: array of bools initialized with the dot values to set, each bool is a dot on the display, from left to right;
  true to show the dot, false to not show the dot;
  array length must match display size.

* __setDot()__

  * display.setDot(index[, dot])

  Modify the stored value by adding or hiding a dot.

  `display`: object of type ShiftDisplay.

  `index`: position on the display to set the dot, starting at 0 for the leftmost;
  if is out of bounds, function does not have any effect.

  `dot`: bool value, true to show the dot, false to not show the dot;
  if is not specified, the default is true.

* __setCustom()__

  * display.setCustom(index, custom)

  Modify the stored value by replacing the character (and dot) with a custom character.

  `display`: object of type ShiftDisplay.

  `index`: position on the display to set the custom character, starting at 0 for the leftmost;
  if is out of bounds, function does not have any effect.

  `custom`: byte with the custom character to set, encoded in abcdefgp format.

* __setAt()__

  * display.setAt(section, value[, alignment])
  * display.setAt(section, valueReal[, decimalPlaces][, alignment])
  * display.setAt(section, customs)
  * display.setAt(section, characters, dots)

  Store a value to show next on the specified section. The section previous value is cleared.

  `display`: object of type ShiftDisplay.

  `section`: position of the section on the display to set the value, starting at 0 for the first;
  if is out of bounds, function does not have any effect.

  `value`: value to set, a number (int, long) or text (char, char array, string object);
  text valid characters are `0-9 a-z A-Z -`, other characters are converted to a space;
  char array must be null-terminated;
  if is too big to fit in the section, it is trimmed according to alignment.
  
  `valueReal`: value to set, a real number (float, double);
  if is too big to fit in the section, it is trimmed according to alignment.

  `alignment`: alignment of the value or valueReal on the section, a constant `ALIGN_LEFT` or `ALIGN_RIGHT` or `ALIGN_CENTER`;
  if is not specified, the default is ALIGN_RIGHT for numbers and ALIGN_LEFT for text.

  `decimalPlaces`: quantity of digits following the decimal point;
  if is not specified, the default is 1.

  `customs`: array of bytes initialized with the custom characters to set, encoded in abcdefgp format, each byte is a character on the section, from left to right;
  array length must match section size.

  `characters`: array of chars initialized with the characters to set, each char is a character on the section, from left to right;
  valid characters are `0-9 a-z A-Z -`, other characters are converted to a space;
  array length must match section size.

  `dots`: array of bools initialized with the dot values to set, each bool is a dot on the section, from left to right;
  true to show the dot, false to not show the dot;
  array length must match section size.

* __setDotAt()__

  * display.setDotAt(section, index[, dot])

  Modify the stored value by adding or hiding a dot in a section.

  `display`: object of type ShiftDisplay.

  `section`: position of the section on the display to set the dot, starting at 0 for the first;
  if is out of bounds, function does not have any effect.

  `index`: position on the section to set the dot, starting at 0 for the leftmost;
  if is out of bounds, function does not have any effect.

  `dot`: bool value, true to show the dot, false to not show the dot;
  if is not specified, the default is true.

* __setCustomAt()__

  * display.setCustomAt(section, index, custom)

  Modify the stored value by replacing the character (and dot) with a custom character in a section.

  `display`: object of type ShiftDisplay.

  `section`: position of the section on the display to set the custom character, starting at 0 for the first;
  if is out of bounds, function does not have any effect.

  `index`: position on the section to set the custom character, starting at 0 for the leftmost;
  if is out of bounds, function does not have any effect.

  `custom`: byte with the custom character to set, encoded in abcdefgp format.

* __update()__

  * display.update()

  Show on the display the stored value. For multiplexed drive, this must be in a loop; for static drive, the value is shown until update()/clear()/show() is called.

  `display`: object of type ShiftDisplay.

* __clear()__

  * display.clear()

  Hide display content, without erasing stored value.
  
  `display`: object of type ShiftDisplay.

* __show()__

  * display.show(time)

  Show on the display the stored value, for the given time, then clear. Blocking function.
  
  `display`: object of type ShiftDisplay.

  `time`: duration in milliseconds to show the value;
  exact time showing will be an under approximation.

### Constants

- ALIGN_LEFT (char)
- ALIGN_RIGHT (char)
- ALIGN_CENTER (char)
- COMMON_ANODE (DisplayType)
- COMMON_CATHODE (DisplayType)
- MULTIPLEXED_DRIVE (DisplayDrive)
- STATIC_DRIVE (DisplayDrive)


## Notes

* __Common Anode & Common Cathode__

TODO

* __Multiplexed Drive & Static Drive__

TODO

* __abcdefgp format__

TODO

* __Decoupling capacitor__

TODO


## Changelog

- master ()
  - NEW: added static drive
  - NEW: added clear() function
  - CHANGE: default decimal places is now 1
  - CHANGE: renamed show() to update() and changed its behaviour
  - CHANGE: display type constants type changed from int to DisplayType
  - CHANGE: dot value is now optional in setDot() and setDotAt()
  - CHANGE: deprecated show(value, time) for simplicity sake
  - DOC: removed 74HC595 references because it works with other shift registers
  - DOC: added type of constants
  - DOC: square brackets for optional args
  - FIX: compiler warnings
- 3.6.1 (17/9/2017)
  - DOC: improved README
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

- [ ] Finish README
- [ ] Example for sectioned display
- [ ] Example for custom characters
- [x] Static Drive
- [ ] Example for static drive
- [ ] Docs for static drive
- [ ] Remove display max size for static drive


## Contacts

- https://github.com/MiguelPynto/ShiftDisplay/
- https://twitter.com/MiguelPynto
- miguelpynto@outlook.com

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
  * ShiftDisplay sd(displayType, displaySize)
  * ShiftDisplay sd(latchPin, clockPin, dataPin, displayType, displaySize)

  Creates a ShiftDisplay object, initializes the library with the interface pins, and sets up with the display properties.

  `sd`: is an object of type ShiftDisplay.

  `latchPin`, `clockPin`, `dataPin`: are the number of the Arduino digital pins connected to the shift registers latch, clock and data pins;
  optional; if not defined, the default pins are 6, 7 and 5 respectively.

  `displayType`: is a constant `COMMON_CATHODE` or `COMMON_ANODE`, depending on the type of your display(s).

  `displaySize`: is the quantity of digits on all displays combined.

### Functions

* __set()__
  * sd.set(value)
  * sd.set(value, alignment)
  * sd.set(value, decimalPlaces)
  * sd.set(value, decimalPlaces, alignment)
  * sd.set(code[])

  Save a value to buffer for showing latter.

  `sd`: is an object of type ShiftDisplay.

  `value`: is the value to save;
  can be a number (int, long, float, double) or text (char, char array, String object);
  for text, valid characters are 0-9 a-z A-Z - space.

  `alignment`: is the alignment of the value on the display;
  optional, constant `ALIGN_LEFT`, `ALIGN_RIGHT` or `ALIGN_CENTER`;
  if not defined, the default is ALIGN_RIGHT for numbers and ALIGN_LEFT for text.

  `decimalPlaces`: is the number of digits following the decimal point;
  optional and only available if the value is a float or double number;
  if not defined, the default is 2.

  `code`: is an array of custom characters, each encoded as a byte where segment A is MSB and DP is LSB;
  array length must match display size.

* __insertDot()__
  * sd.insertDot(index)

  Add dot to buffer.

  `index`: is the position on the display to insert the dot, starting at 0 for the leftmost;
  if is invalid, does not have any effect.

* __removeDot()__
  * sd.removeDot(index)

  Remove dot from buffer.

  `index`: is the position on the display to remove the dot, starting at 0 for the leftmost;
  if is invalid, does not have any effect.

* __loadDots()__
  * sd.loadDots(values[])

  Modify dots in buffer.

  `values`: is the array defining if a dot is shown (true) or hide (false) for every position on the display (as the array index), starting at 0 for the leftmost.

* __show()__
  * sd.show()
  * sd.show(time)

  Show buffer value on the display.

  `sd`: is an object of type ShiftDisplay.

  `time`: is the time in milliseconds for the value to be shown on the display;
  has to be >= 10;
  optional; if not defined, the value is shown for a single iteration;
  exact time showing will be an under aproximation.

* __show()__
  * sd.show(value, time)
  * sd.show(value, time, alignment)
  * sd.show(value, time, decimalPlaces)
  * sd.show(value, time, decimalPlaces, alignment)
  * sd.show(code[], time)

  Save a value to buffer and show it on the display.

  `sd`: is an object of type ShiftDisplay.

  `value`: is the value to save and show;
  can be a number (int, long, float, double) or text (char, char array, String object);
  for text, valid characters are 0-9 a-z A-Z - space.

  `time`: is the time in milliseconds for the value to be shown on the display;
  has to be >= 10;
  exact time showing will be an under aproximation.

  `alignment`: is the alignment of the value on the display;
  optional, constant `ALIGN_LEFT`, `ALIGN_RIGHT` or `ALIGN_CENTER`;
  if not defined, the default is ALIGN_RIGHT for numbers and ALIGN_LEFT for text.

  `decimalPlaces`: is the number of digits following the decimal point;
  optional and only available if the value is a float or double number;
  if not defined, the default is 2.

  `code`: is an array of custom characters, each encoded as a byte where segment A is MSB and DP is LSB.
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
ShiftDisplay sd(6, 7, 5, COMMON_CATHODE, 3);

void setup() {
	for (int i = 3; i > 0; i--) {
		sd.show(i, 400, ALIGN_CENTER); // save number and show it for 400ms
		sd.insertDot(1); // add dot to saved number
		sd.show(400); // show number with dot for 400ms
	}
	sd.set("GO"); // save "GO" to buffer
}

void loop() {
	sd.show(); // show "GO" while in loop
}
```


## Changelog

- next ()
  - Feature: custom characters
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

- [x] Custom characters
- [ ] Multiple logical displays?


## Contacts

miguelpynto@outlook.com

https://miguelpynto.github.io/ShiftDisplay/

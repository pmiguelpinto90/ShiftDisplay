# ShiftDisplay
Arduino library for driving multiple digit 7-segment displays using shift registers

Hardware
- Multiple-digit 7-segment LED display (common anode or common cathode)
- 74HC595 Shift Register (other 8-bit shift registers should work)

Install
- Download library
- Extract to Arduino library folder

Wiring

Functions

- ShiftDisplay s(abcd)

- print(value, time)
Shows a value in the display during the given time in milliseconds.
The value can be an integer or a String
Returns true if there was space to write

- print(value, decimalCase, time)

- printMenu(character, value, time)


Example

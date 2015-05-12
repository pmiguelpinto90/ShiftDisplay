# ShiftDisplay
Arduino library for driving multiple digit 7-segment displays using shift registers

## Hardware
- Multiple-digit 7-segment LED display (common anode or common cathode)
- 74HC595 Shift Register (other 8-bit shift registers should work)

## Install
- Download library
- Extract to Arduino library folder

## Wiring

## API

```
ShiftDisplay(int latchPin, int clkPin, int dataPin, bool commonCathode, int nDigits)

Creates object display.
latchPin, clkPin and dataPin are the shift register pins connected to the Arduino.
commonCathode is a boolean for the led type.
nDigits are the number of digits of the led.
```

```
bool print(int value, int milliseconds)

Displays integer value, right aligned in display, for the given milliseconds.
Returns true if displayed whole number.

```

```
bool print(float value, int nDecimalPlaces, int milliseconds)

Displays float value, right aligned in display, rounded to nDecimalPlaces,
for the given milliseconds.
Returns true if displayed whole number.
```

```
bool print(String text, int milliseconds)

Displays text, left aligned in display, for the given milliseconds.
Accepted characters for string are A-Z, a-z, 0-9, -, space.
Returns true if displayed whole string.
```

```
bool printMenu(char c, int value, int milliseconds)

Displays character c left aligned in display, and integer value right aligned
in display, for the given milliseconds.
Returns true if displayed whole value.
```

## Example

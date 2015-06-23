/*
  ShiftDisplayMini
  2.0.0 (24/06/2015)
  https://github.com/Pyntoo/ShiftDisplay
*/


#include "Arduino.h"
#include "ShiftDisplayMini.h"
#include "constants.h"


// CONSTRUCTOR
ShiftDisplayMini::ShiftDisplayMini(int latchPin, int clockPin, int dataPin, bool commonCathode, int displaySize) {
	pinMode(latchPin, OUTPUT);
	pinMode(clockPin, OUTPUT);
	pinMode(dataPin, OUTPUT);
	_latchPin = latchPin;
	_clockPin = clockPin;
	_dataPin = dataPin;
	_commonCathode = commonCathode;
	_displaySize = displaySize;
}


// PRIVATE
// Calculate power of number by exponent.
int ShiftDisplayMini::power(int value, int exponent) {
	return round(pow(value, exponent));
}

// PRIVATE
// Clears display
void ShiftDisplayMini::clear() {
	digitalWrite(_latchPin, LOW);
	shiftOut(_dataPin, _clockPin, MSBFIRST, 0); // both ends of led with same value
	shiftOut(_dataPin, _clockPin, MSBFIRST, 0);
	digitalWrite(_latchPin, HIGH);
}

// PRIVATE
// Displays byte array
// Pre: characters array size = display number of digits
void ShiftDisplayMini::printPov(byte characters[], int time) {
	unsigned long start = millis();
	while (millis() - start < time) {
		for (int i = 0; i < _displaySize; i++) {
			digitalWrite(_latchPin, LOW);

			// shift data for display register
			byte out = _commonCathode ? ~DISPLAYS[i] : DISPLAYS[i];
			shiftOut(_dataPin, _clockPin, MSBFIRST, out);

			// shift data for character register
			shiftOut(_dataPin, _clockPin, MSBFIRST, characters[i]);

			digitalWrite(_latchPin, HIGH);
			delay(POV);
		}
	}
	clear();
}

// PUBLIC
// Display an integer value, right aligned in the display,
// for the given time in milliseconds.
void ShiftDisplayMini::print(int value, int time) {
	bool negative = value < 0;
	byte characters[_displaySize];
	int i = 0;

	// tranform number into positive
	if (negative)
		value = value * -1;

	// store digits from number in array
	do { // if number is zero, prints single 0
		int digit = value % 10;
		characters[i++] = _commonCathode ? DIGITS[digit]: ~DIGITS[digit];
		value /= 10;
	} while (value && i < _displaySize);

	// place minus character on left of number
	if (negative && i < _displaySize)
		characters[i++] = _commonCathode ? MINUS : ~MINUS;

	// fill remaining array with empty
	while (i < _displaySize)
		characters[i++] = _commonCathode ? SPACE : ~SPACE;

	printPov(characters, time);
}

// PUBLIC
// Display a float value, rounded to specified decimal places,
// right aligned in the display, for the given time in milliseconds.
void ShiftDisplayMini::print(float value, int decimalPlaces, int time) {

	// if no decimal places, print int
	if (decimalPlaces == 0) {
		int n = round(value);
		print(n, time);
		return;
	}

	bool negative = value < 0;
	byte characters[_displaySize];
	int i = 0;

	// transform number in positive
	if (negative)
		value = value * -1;

	// get digits rounded without comma from value
	long newValue = round(value * power(10, decimalPlaces));

	// store digits in array
	while ((newValue || i <= decimalPlaces) && i < _displaySize) {
		int digit = newValue % 10;
		characters[i++] = _commonCathode ? DIGITS[digit] : ~DIGITS[digit];
		newValue /= 10;
	}

	// place decimal point in first integer
	if (decimalPlaces < _displaySize)
		characters[decimalPlaces] = characters[decimalPlaces] + DOT;

	// place minus character on left of numbers
	if (negative && i < _displaySize)
		characters[i++] = _commonCathode ? MINUS : ~MINUS;

	// fill remaining characters with empty
	while (i < _displaySize)
		characters[i++] = _commonCathode ? SPACE : ~SPACE;

	printPov(characters, time);
}

// PUBLIC
// Display text, left aligned in the display, for the given time in milliseconds.
// Accepted characters for string are A-Z, a-z, 0-9, -, space.
void ShiftDisplayMini::print(String text, int time) {
	byte characters[_displaySize];
	int i = _displaySize - 1; // for inverse array iteration
	int j = 0; // for text iteration

	// get characters from text
	while (j < text.length() && i >= 0) {
		char c = text[j++];

		byte out;
		if (c >= 'A' && c <= 'Z')
			out = LETTERS[c - 'A'];
		else if (c >= 'a' && c <= 'z')
			out = LETTERS[c - 'a'];
		else if (c >= '0' && c <= '9')
			out = DIGITS[c - '0'];
		else if (c == '-')
			out = MINUS;
		else
			out = SPACE;
		characters[i--] = _commonCathode ? out : ~out;
	}

	// fill remaining right characters with empty
	while (i >= 0)
		characters[i--] = _commonCathode ? SPACE : ~SPACE;

	printPov(characters, time);
}

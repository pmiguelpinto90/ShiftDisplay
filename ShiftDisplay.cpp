/*
  ShiftDisplay
  2.0.0 (24/06/2015)
  https://github.com/Pyntoo/ShiftDisplay
*/


#include "Arduino.h"
#include "ShiftDisplay.h"
#include "constants.h"


// CONSTRUCTORS ********************************************************************

// Simple constructor with 3 output pins from Arduino. Fade animations disabled.
ShiftDisplay::ShiftDisplay(int latchPin, int clockPin, int dataPin, bool commonCathode, int displaySize) {
	pinMode(latchPin, OUTPUT);
	pinMode(clockPin, OUTPUT);
	pinMode(dataPin, OUTPUT);
	_latchPin = latchPin;
	_clockPin = clockPin;
	_dataPin = dataPin;
	_outputEnablePin = 0;
	_commonCathode = commonCathode;
	_displaySize = displaySize;
	_nShiftRegisters = (displaySize-1)/8 + 2;
	
}

// Constructor with 4 output pins from Arduino. Fade animations possible.
ShiftDisplay::ShiftDisplay(int latchPin, int clockPin, int dataPin, int outputEnablePin, bool commonCathode, int displaySize) {
	pinMode(latchPin, OUTPUT);
	pinMode(clockPin, OUTPUT);
	pinMode(dataPin, OUTPUT);
	_latchPin = latchPin;
	_clockPin = clockPin;
	_dataPin = dataPin;
	_outputEnablePin = outputEnablePin;
	_commonCathode = commonCathode;
	_displaySize = displaySize;
	_nShiftRegisters = (displaySize-1)/8 + 2;
}


// PRIVATE FUNCTIONS ***************************************************************

// Calculate power of number by exponent.
int ShiftDisplay::power(int value, int exponent) {
	return round(pow(value, exponent));
}

// Count the number of digits in an integer value.
// If the number is negative, the minus also counts as a digit.
int ShiftDisplay::countDigits(long value) {
	if (value < 0)
		return 1 + countDigits(-value);
	if (value < 10)
		return 1;
	return 1 + countDigits(value / 10);
}

// Count the number of integer digits in a float value.
// If the number is negative zero, the minus and zero count as two digits.
int ShiftDisplay::countIntegerDigits(float value) {
	if (value > -1 && value < 0)
		return 2;
	return countDigits((int) value);
}

// Count the number of characters in a String.
int ShiftDisplay::countCharacters(String text) {
	return text.length();
}

// Extract a number to an array of byte characters, from most significant to less.
// If the number is negative also place the minus character.
void ShiftDisplay::getDigits(long number, byte digits[], int nDigits) {
	bool negative = number < 0;
	if (negative)
		number = -number;

	int i = nDigits - 1;
	do { // from least to most, inverted index
		int digit = number % 10;
		digits[i--] = _commonCathode ? DIGITS[digit]: ~DIGITS[digit];
		number /= 10;
	} while (i >= 0);

	if (negative)
		digits[0] = _commonCathode ? MINUS : ~MINUS;
}

// Extract a string to an array of byte characters, from left to right.
// An unknown character will be replaced with a space character.
void ShiftDisplay::getCharacters(String text, byte characters[], int nCharacters) {
	for (int i = 0; i < nCharacters; i++) {
		char c = text[i];
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
		characters[i] = _commonCathode ? out : ~out;
	}
}

// Arrange characters for printing in phisical display.
void ShiftDisplay::arrangeCharacters(byte characters[], int nCharacters, byte display[], int alignment) {
	int begin; // left side of display, highest index
	int end; // right side of display, lowest index
	if (alignment == ALIGNMENT_LEFT) {
		begin = _displaySize - 1;
		end = _displaySize - nCharacters;
	} else if (alignment == ALIGNMENT_RIGHT) {
		begin = nCharacters - 1;
		end = 0;
	} else { // alignment == ALIGNMENT_CENTER
		// TODO
		// int displayCenter = _displaySize / 2;
		// int charactersCenter = ((nCharacters + 1) / 2) - 1;
		// begin = displayCenter + charactersCenter + 1;
		// end = begin - nCharacters + 1;
		begin = 0;
		end = 0;
	}
	if (begin > _displaySize - 1) // out of bounds
		begin = _displaySize - 1;
	if (end < 0) // out of bounds
		end = 0;

	for (int i = _displaySize - 1, j = 0; i >= 0; i--) {
		if (i <= begin && i >= end)
			display[i] = characters[j++];
		else
			display[i] = _commonCathode ? SPACE : ~SPACE;
	}
}

// TODO
void ShiftDisplay::arrangeCharactersScroll(byte characters[], int nCharacters, byte display[], int alignment, int step) {

}

// Clears display
void ShiftDisplay::clear() {
	digitalWrite(_latchPin, LOW);
	for (int i = 0; i < _nShiftRegisters; i++)
		shiftOut(_dataPin, _clockPin, MSBFIRST, 0); // led io with same value
	digitalWrite(_latchPin, HIGH);
}

// Displays byte array
// Pre: characters array size = display number of digits
void ShiftDisplay::printDisplay(byte display[], int time, int animation) {
	unsigned long start = millis();
	while (millis() - start < time) {
		for (int i = 0; i < _displaySize; i++) {
			digitalWrite(_latchPin, LOW);

			// shift data for all display registers
			for (int j = _nShiftRegisters - 2; j >= 0; j--) {
				byte pos;
				if (i/8 == j) // shift register where correct display is connected
					pos = _commonCathode ? ~DISPLAYS[i] : DISPLAYS[i];
				else
					pos = _commonCathode ? ~DISPLAYS_OFF : DISPLAYS_OFF;
				shiftOut(_dataPin, _clockPin, MSBFIRST, pos);
			}

			// shift data for character register
			shiftOut(_dataPin, _clockPin, MSBFIRST, display[i]);

			digitalWrite(_latchPin, HIGH);
			delay(POV);
		}
	}
	clear();
}

// Arrange characters by specified alignment in an array representing the display,
// and send it to print.
void ShiftDisplay::printCharacters(byte characters[], int nCharacters, int time, int animation, int alignment) {
	byte display[_displaySize];
	// TODO scroll
	// if (animation == ANIMATION_SCROLL) {
		// for (int i = 0; i < nCharacters + _displaySize; i++) {
			// arrangeCharactersScroll(characters, nCharacters, display, alignment, i);
			// printDisplay(display, time, animation);
		// }
	// } else {
		arrangeCharacters(characters, nCharacters, display, alignment);
		printDisplay(display, time, animation);
	// }
}


// PUBLIC FUNCTIONS ****************************************************************

// Show an integer value in the display, for the given time in milliseconds,
// with specified animation and alignment constants.
void ShiftDisplay::print(int value, int time, int animation, int alignment) {
	int nCharacters = countDigits(value);
	byte characters[nCharacters];
	getDigits(value, characters, nCharacters);
	printCharacters(characters, nCharacters, time, animation, alignment);
}

// Show a float value, rounded to specified number of decimal places, for the
// given time in milliseconds, with specified animation and alignment constants.
void ShiftDisplay::print(float value, int decimalPlaces, int time, int animation, int alignment) {
	if (decimalPlaces == 0) {
		print(round(value), time, animation, alignment);
		return;
	}
	long newValue = round(value * power(10, decimalPlaces));

	int nCharacters = countIntegerDigits(value) + decimalPlaces;
	byte characters[nCharacters];
	getDigits(newValue, characters, nCharacters);
	int dotPosition = nCharacters - decimalPlaces - 1;
	characters[dotPosition] += DOT;
	printCharacters(characters, nCharacters, time, animation, alignment);
}

// Show text in the display, for the given time in milliseconds,
// with specified animation and alignment constants.
// Accepted characters are A-Z, a-z, 0-9, -, space.
void ShiftDisplay::print(String text, int time, int animation, int alignment) {
	int nCharacters = countCharacters(text);
	byte characters[nCharacters];
	getCharacters(text, characters, nCharacters);
	printCharacters(characters, nCharacters, time, animation, alignment);
}

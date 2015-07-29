/*
  ShiftDisplay 2.2.0 (14/07/2015)
  https://github.com/Pyntoo/ShiftDisplay
*/


#include "Arduino.h"
#include "ShiftDisplay.h"
#include "ShiftDisplayConstants.h"


// CONSTRUCTORS ********************************************************************

// Creates default ShiftDisplay object using 3 pins, fade animations disabled.
// latchPin, clockPin and dataPin are the shift register inputs connected to the
// Arduino digital outputs.
// commonCathode is true if the led type is common cathode, false if it's common anode.
// displayLength is the quantity of digits of all displays together.
ShiftDisplay::ShiftDisplay(int latchPin, int clockPin, int dataPin, bool commonCathode, int displayLength) {
	pinMode(latchPin, OUTPUT);
	pinMode(clockPin, OUTPUT);
	pinMode(dataPin, OUTPUT);
	_latchPin = latchPin;
	_clockPin = clockPin;
	_dataPin = dataPin;
	_outputEnablePin = 0;
	_commonCathode = commonCathode;
	_displayLength = displayLength;
	_nShiftRegisters = (displayLength-1)/8 + 2;
}


// Creates advanced ShiftDisplay object using 4 pins, fade animations enabled.
// latchPin, clockPin, dataPin and outputEnablePin are the shift register inputs
// connected to the Arduino digital outputs. outputEnablePin must be connected to a
// PWM pin.
// commonCathode is true if the led type is common cathode, false if it's common anode.
// displayLength is the quantity of digits of all displays together.
ShiftDisplay::ShiftDisplay(int latchPin, int clockPin, int dataPin, int outputEnablePin, bool commonCathode, int displayLength) {
	pinMode(latchPin, OUTPUT);
	pinMode(clockPin, OUTPUT);
	pinMode(dataPin, OUTPUT);
	_latchPin = latchPin;
	_clockPin = clockPin;
	_dataPin = dataPin;
	_outputEnablePin = outputEnablePin;
	_commonCathode = commonCathode;
	_displayLength = displayLength;
	_nShiftRegisters = (displayLength-1)/8 + 2;
}


// PRIVATE FUNCTIONS ***************************************************************

// Calculate power of number by exponent.
int power(int value, int exponent) {
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
void ShiftDisplay::getCharacters(String text, byte characters[], int charactersLength) {
	for (int i = 0; i < charactersLength; i++) {
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
			out = BLANK;
		characters[i] = _commonCathode ? out : ~out;
	}
}


// Clears display
void ShiftDisplay::clear() {
	digitalWrite(_latchPin, LOW);
	for (int i = 0; i < _nShiftRegisters; i++)
		shiftOut(_dataPin, _clockPin, MSBFIRST, 0); // led io with same value
	digitalWrite(_latchPin, HIGH);
}


// Displays byte array
// Pre: characters array length = display number of digits
void ShiftDisplay::showDisplay(byte display[], int time) {
	unsigned long start = millis();
	while (millis() - start < time) {
		for (int i = 0; i < _displayLength; i++) { // for each digit of display
			digitalWrite(_latchPin, LOW);

			// shift data for all display registers
			for (int j = _nShiftRegisters - 2; j >= 0; j--) {
				byte pos;
				if (i/8 == j) // shift register where correct display is connected
					pos = _commonCathode ? ~DISPLAYS[i] : DISPLAYS[i];
				else
					pos = _commonCathode ? ~DISPLAYS_OFF : DISPLAYS_OFF;
				shiftOut(_dataPin, _clockPin, LSBFIRST, pos);
			}

			// shift data for character register
			shiftOut(_dataPin, _clockPin, MSBFIRST, display[i]);

			digitalWrite(_latchPin, HIGH);
			delay(POV);
		}
	}
	clear();
}


// Move all characters in display one position to left or right.
void ShiftDisplay::shiftDisplay(byte display[], bool toRight) {
	int i;

	// shift all characters, except the last witch is moved to out of view
	if (toRight) {
		for (i = _displayLength - 1; i > 0 ; i--)
			display[i] = display[i - 1];
	} else { // to left
		for (i = 0; i < _displayLength - 1; i++)
			display[i] = display[i + 1];
	}

	// new character from out of view
	display[i] = _commonCathode ? BLANK : ~BLANK;
}


// Arrange characters for printing in phisical display.
void ShiftDisplay::prepareDisplay(byte characters[], int charactersLength, byte display[], int alignment) {
	int begin; // left side of display, lowest index
	int end; // right side of display, highest index

	// calculate where characters are in the display, according to alignment
	switch (alignment) {
		case ALIGNMENT_LEFT:
			begin = 0;
			end = charactersLength - 1;
			break;

		case ALIGNMENT_RIGHT:
			begin = _displayLength - charactersLength;
			end = _displayLength - 1;
			break;

		case ALIGNMENT_CENTER:
			begin = (_displayLength - charactersLength) / 2;
			end = begin + charactersLength - 1;
			break;
	}

	// check out of bounds
	if (begin < 0)
		begin = 0;
	if (end > _displayLength - 1)
		end = _displayLength - 1;

	// fill display array with the right characters or empty spaces
	for (int i = 0, j = 0; i < _displayLength; i++) {
		if (i >= begin && i <= end)
			display[i] = characters[j++];
		else
			display[i] = _commonCathode ? BLANK : ~BLANK;
	}
}


// Arrange characters by alignment and animation and send it to print.
void ShiftDisplay::printCharacters(byte characters[], int charactersLength, int time, int animation, int alignment) {
	byte display[_displayLength]; // display formated array

	// initialize display array by alignment
	prepareDisplay(characters, charactersLength, display, alignment);

	switch (animation) {
		int steps; // this has to be here instead of inside a case
		case ANIMATION_EXIT_LEFT:
		case ANIMATION_EXIT_RIGHT:
			// same side or not
			steps = (animation == alignment) ? charactersLength : _displayLength;
			time /= steps;
			showDisplay(display, time);
			for (int i = 1; i < steps; i++) {
				shiftDisplay(display, animation == ANIMATION_EXIT_RIGHT);
				showDisplay(display, time);
			}
			break;
		case ANIMATION_SCROLL:
			// for (int i = )

			break;
		// case ANIMATION_FADE_IN:
			// fadeDisplay();
		case ANIMATION_NONE:
			showDisplay(display, time);
			break;
	}
}


// PUBLIC FUNCTIONS ****************************************************************

// Show an integer value for 2 seconds, right aligned in the display.
void ShiftDisplay::print(int value) {
	print(value, 2000, ALIGNMENT_RIGHT, ANIMATION_NONE);
}


// Show an integer value in the display, for the given time in milliseconds,
// with specified alignment and animation constants.
void ShiftDisplay::print(int value, int time, int alignment, int animation) {
	int charactersLength = countDigits(value);
	byte characters[charactersLength];
	getDigits(value, characters, charactersLength);
	printCharacters(characters, charactersLength, time, animation, alignment);
}


// Show a float value, rounded to specified number of decimal places, for the
// given time in milliseconds, with specified alignment and animation constants.
void ShiftDisplay::print(float value, int decimalPlaces, int time, int alignment, int animation) {
	if (decimalPlaces == 0) {
		print(round(value), time, animation, alignment);
		return;
	}
	long newValue = round(value * power(10, decimalPlaces));

	int charactersLength = countIntegerDigits(value) + decimalPlaces;
	byte characters[charactersLength];
	getDigits(newValue, characters, charactersLength);
	int dotPosition = charactersLength - decimalPlaces - 1;
	characters[dotPosition] += DOT;
	printCharacters(characters, charactersLength, time, animation, alignment);
}


// Show text in the display, for the given time in milliseconds,
// with specified alignment and animation constants.
// Accepted characters are A-Z, a-z, 0-9, -, space.
void ShiftDisplay::print(String text, int time, int alignment, int animation) {
	int charactersLength = countCharacters(text);
	byte characters[charactersLength];
	getCharacters(text, characters, charactersLength);
	printCharacters(characters, charactersLength, time, animation, alignment);
}

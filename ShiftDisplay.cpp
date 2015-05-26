/*

  ShiftDisplay
  1.0.0 (26/05/2015)
  Arduino library for driving multiple digit 7-segment displays using shift registers
  
  Pyntoo
  r.pynto@gmail.com

  Released into the public domain on May 9, 2015

*/

#include "Arduino.h"
#include "ShiftDisplay.h"


const int POV = 5; // delay for persistence of vision

// characters encoding
const byte DIGITS[10] = {
	//GFEDCBA
	B00111111, // 0
	B00000110, // 1
	B01011011, // 2
	B01001111, // 3
	B01100110, // 4
	B01101101, // 5
	B01111101, // 6
	B00000111, // 7
	B01111111, // 8
	B01100111 // 9
};
const byte LETTERS[26] = {
	//GFEDCBA
	B01110111, // a
	B01111100, // b
	B00111001, // c
	B01011110, // d
	B01111001, // e
	B01110001, // f
	B00111101, // g
	B01110110, // h
	B00110000, // i
	B00011110, // j
	B01110101, // k
	B00111000, // l
	B00110111, // m
	B01010100, // n
	B01011100, // o
	B01110011, // p
	B01100111, // q
	B00110001, // r
	B01101101, // s
	B01111000, // t
	B00011100, // u
	B00111110, // v
	B01111110, // w
	B00110110, // x
	B01101110, // y
	B01011011 // z
};
const byte MINUS = B01000000;
const byte DOT = B10000000;
const byte SPACE = B00000000;

// displays encoding
const byte DISPLAYS[8] = {
	B00000001,
	B00000010,
	B00000100,
	B00001000,
	B00010000,
	B00100000,
	B01000000,
	B10000000
};
const byte DISPLAYS_OFF = {
	B00000000,
};


// CONSTRUCTOR
ShiftDisplay::ShiftDisplay(int latchPin, int clockPin, int dataPin, bool commonCathode, int nDigits) {
	pinMode(latchPin, OUTPUT);
	pinMode(clockPin, OUTPUT);
	pinMode(dataPin, OUTPUT);
	_latchPin = latchPin;
	_clockPin = clockPin;
	_dataPin = dataPin;
	// _outputEnablePin = 0;
	_commonCathode = commonCathode;
	_nCharacters = nDigits;
	_nShiftRegisters = (nDigits-1)/8 + 2;
}


// PRIVATE
// Calculate power of number by exponent
int ShiftDisplay::power(int value, int exponent) {
	return round(pow(value, exponent));
}

// PRIVATE
// Clears display
void ShiftDisplay::clear() {
	digitalWrite(_latchPin, LOW);
	for (int i = 0; i < _nShiftRegisters; i++)
		shiftOut(_dataPin, _clockPin, MSBFIRST, 0); // both ends of led with same value
	digitalWrite(_latchPin, HIGH);
}

// PRIVATE
// Displays byte array
// Pre: characters array size = display number of digits
void ShiftDisplay::printx(byte characters[], int time) {
	unsigned long start = millis();
	while (millis() - start < time) {
		for (int i = 0; i < _nCharacters; i++) {
			digitalWrite(_latchPin, LOW);

			// shift data for all display registers
			for (int j = _nShiftRegisters - 2; j >= 0; j--) {
				byte out;
				if (i/8 == j) // shift register where correct display is connected
					out = _commonCathode ? ~DISPLAYS[i] : DISPLAYS[i];
				else
					out = _commonCathode ? ~DISPLAYS_OFF : DISPLAYS_OFF;
				shiftOut(_dataPin, _clockPin, MSBFIRST, out);
			}

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
void ShiftDisplay::print(int value, int time) {
	bool negative = value < 0;
	byte characters[_nCharacters];
	int i = 0;

	// tranform number into positive
	if (negative)
		value = value * -1;

	// store digits from number in array
	do { // if number is zero, prints single 0
		int digit = value % 10;
		characters[i++] = _commonCathode ? DIGITS[digit]: ~DIGITS[digit];
		value /= 10;
	} while (value && i < _nCharacters);

	// place minus character on left of number
	if (negative && i < _nCharacters)
		characters[i++] = _commonCathode ? MINUS : ~MINUS;

	// fill remaining array with empty
	while (i < _nCharacters)
		characters[i++] = _commonCathode ? SPACE : ~SPACE;

	printx(characters, time);
}

// PUBLIC
// Display a float value, rounded to specified decimal places,
// right aligned in the display, for the given time in milliseconds.
void ShiftDisplay::print(float value, int decimalPlaces, int time) {

	// if no decimal places, print int
	if (decimalPlaces == 0) {
		int n = round(value);
		return print(n, time); // TODO test return exit
	}

	bool negative = value < 0;
	byte characters[_nCharacters];
	int i = 0;

	// transform number in positive
	if (negative)
		value = value * -1;

	// get digits rounded without comma from value
	long newValue = round(value * power(10, decimalPlaces));

	// store digits in array
	do {
		int digit = newValue % 10;
		characters[i++] = _commonCathode ? DIGITS[digit] : ~DIGITS[digit];
		newValue /= 10;
	} while (newValue && i < _nCharacters);

	// place decimal point in first integer
	if (decimalPlaces < _nCharacters)
		characters[decimalPlaces] = characters[decimalPlaces] + DOT;

	// place minus character on left of numbers
	if (negative && i < _nCharacters)
		characters[i++] = _commonCathode ? MINUS : ~MINUS;

	// fill remaining characters with empty
	while (i < _nCharacters)
		characters[i++] = _commonCathode ? SPACE : ~SPACE;

	printx(characters, time);
}

// PUBLIC
// Display text, left aligned in the display, for the given time in milliseconds.
// Accepted characters for string are A-Z, a-z, 0-9, -, space.
void ShiftDisplay::print(String text, int time) {
	byte characters[_nCharacters];
	int i = _nCharacters - 1; // for inverse array iteration
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

	printx(characters, time);
}

// PUBLIC
// Display a character, left aligned in the display, and an integer value,
// right aligned in the display, for the given time in milliseconds.
void ShiftDisplay::printMenu(char c, int value, int time) {
	bool negative = value < 0;
	byte characters[_nCharacters];
	int i = 0;

	// transform value in positive
	if (negative)
		value = value * -1;

	// get digits from number
	do {
		int digit = value % 10;
		characters[i++] = _commonCathode ? DIGITS[digit] : ~DIGITS[digit];
		value /= 10;
	} while (value && i < _nCharacters - 1);

	// place minus character at numbers left
	if (negative && i < _nCharacters - 1)
		characters[i++] = _commonCathode ? MINUS : ~MINUS;	

	// fill remaining characters with empty, except last
	while (i < _nCharacters - 1)
		characters[i++] = _commonCathode ? SPACE : ~SPACE;

	// place letter in last, with dot
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
	out = out + DOT;
	characters[i] = _commonCathode ? out : ~out;

	printx(characters, time);
}

// PUBLIC
// Display a character, left aligned in the display, and a float value,
// right aligned in the display, rounded to specified decimal places,
// for the given time in milliseconds.
void printMenu(char c, float value, int decimalPlaces, int time) {
	
	// if no decimal places, printMenu int
	if (decimalPlaces == 0) {
		int n = round(value);
		return printMenu(c, n, time);
	}

	bool negative = value < 0;
	byte characters[_nCharacters];
	int i = 0;

	// transform number in positive
	if (negative)
		value = value * -1;

	// get digits rounded without comma from value
	long newValue = round(value * power(10, decimalPlaces));

	// store digits in array
	do {
		int digit = newValue % 10;
		characters[i++] = _commonCathode ? DIGITS[digit] : ~DIGITS[digit];
		newValue /= 10;
	} while (newValue && i < _nCharacters - 1);

	// place decimal point in first integer
	if (decimalPlaces < _nCharacters - 1)
		characters[decimalPlaces] = characters[decimalPlaces] + DOT;

	// place minus character on left of numbers
	if (negative && i < _nCharacters - 1)
		characters[i++] = _commonCathode ? MINUS : ~MINUS;

	// fill remaining characters with empty
	while (i < _nCharacters - 1)
		characters[i++] = _commonCathode ? SPACE : ~SPACE;

	// place letter in last, with dot
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
	out = out + DOT;
	characters[i] = _commonCathode ? out : ~out;

	printx(characters, time);
}

// PUBLIC
// Display a character, left aligned in the display, and text, right aligned
// in the display, for the given time in milliseconds.
void printMenu(char c, String text, int time) {
	byte characters[_nCharacters];
	int i = 0;

	// get characters from text
	while (i < text.length() && i < _nCharacters - 1) {
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
		characters[i++] = _commonCathode ? out : ~out;
	}

	// fill remaining right characters with empty
	while (i < _nCharacters - 1)
		characters[i++] = _commonCathode ? SPACE : ~SPACE;

	printx(characters, time);
}

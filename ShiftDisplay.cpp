/*
  ShiftDisplay.cpp
  Library for driving multiple digit 7-segment displays using shift registers
  Created by Pyntoo, May 9, 2015
  Released into the public domain
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

ShiftDisplay::ShiftDisplay(int latchPin, int clkPin, int dataPin, bool commonCathode, int nDigits) {
	pinMode(latchPin, OUTPUT);
	pinMode(clkPin, OUTPUT);
	pinMode(dataPin, OUTPUT);
	_latchPin = latchPin;
	_clkPin = clkPin;
	_dataPin = dataPin;
	_commonCathode = commonCathode;
	_nCharacters = nDigits;
	_nShiftRegisters = (nDigits-1)/8 + 2;
}

/*
PRIVATE
Calculate power of number by exponent
*/
int ShiftDisplay::power(int value, int exponent) {
	return round(pow(value, exponent));
}

/*
PRIVATE
Clears display
*/
void ShiftDisplay::clear() {
	digitalWrite(_latchPin, LOW);
	for (int i = 0; i < _nShiftRegisters; i++)
		shiftOut(_dataPin, _clkPin, MSBFIRST, 0); // both ends of led with same value
	digitalWrite(_latchPin, HIGH);
}

/*
PRIVATE
Displays byte array
Pre: characters array size = display number of digits
*/
void ShiftDisplay::printx(int milliseconds, byte characters[]) {
	unsigned long start = millis();
	while (millis() - start < milliseconds) {
		for (int i = 0; i < _nCharacters; i++) {
			digitalWrite(_latchPin, LOW);

			// shift data for all display registers
			for (int j = _nShiftRegisters - 2; j >= 0; j--) {
				byte out;
				if (i/8 == j) // shift register where correct display is connected
					out = _commonCathode ? ~DISPLAYS[i] : DISPLAYS[i];
				else
					out = _commonCathode ? ~DISPLAYS_OFF : DISPLAYS_OFF;
				shiftOut(_dataPin, _clkPin, MSBFIRST, out);
			}

			// shift data for character register
			shiftOut(_dataPin, _clkPin, MSBFIRST, characters[i]);

			digitalWrite(_latchPin, HIGH);
			delay(POV);
		}
	}
	clear();
}

/*
PUBLIC
Displays integer value, right aligned in the display, for the given milliseconds
Returns true if displayed whole number
*/
bool ShiftDisplay::print(int value, int milliseconds) {
	bool sucess = true;
	int negative = value < 0;
	byte characters[_nCharacters];
	int i = 0;

	// tranform number into positive
	if (negative)
		value = value * -1;

	// store digits from number in array
	do { // if number is zero, prints single 0
		int digit = value % 10;
		characters[i++] = _commonCathode ? DIGITS[digit]: ~DIGITS[digit];
		value = value / 10;
	} while (value != 0 && i < _nCharacters);
	if (value != 0)
		sucess = false;

	// place minus character on left of number
	if (negative) {
		if (i < _nCharacters)
			characters[i++] = _commonCathode ? MINUS : ~MINUS;
		else
			sucess = false;
	}

	// fill remaining array with empty
	while (i < _nCharacters)
		characters[i++] = _commonCathode ? SPACE : ~SPACE;

	printx(milliseconds, characters);

	return sucess;
}

/*
PUBLIC
Displays float value, right aligned in display, rounded to nDecimalPlaces,
for the given milliseconds
Returns true if displayed whole number
*/
bool ShiftDisplay::print(float value, int nDecimalPlaces, int milliseconds) {

	// if no decimal places, print int
	if (nDecimalPlaces == 0) {
		int n = round(value);
		return print(n, milliseconds);
	}

	bool sucess = true;
	bool negative = value < 0;
	byte characters[_nCharacters];
	int i = 0;

	// transform number in positive
	if (negative)
		value = value * -1;

	// remove decimal point and convert in integer
	int newNumber = round(value * power(10, nDecimalPlaces));

	// store digits from number in array
	do {
		int digit = newNumber % 10;
		characters[i++] = _commonCathode ? DIGITS[digit] : ~DIGITS[digit];
		newNumber = newNumber / 10;
	} while (newNumber != 0 && i < _nCharacters);
	if (newNumber != 0)
		sucess = false;

	// place decimal point
	if (nDecimalPlaces < _nCharacters)
		characters[nDecimalPlaces] = characters[nDecimalPlaces] + DOT;
	else
		sucess = false;

	// place minus character on left of numbers
	if (negative) {
		if (i < _nCharacters)
			characters[i++] = _commonCathode ? MINUS : ~MINUS;
		else
			sucess = false;
	}

	// fill remaining characters with empty
	while (i < _nCharacters)
		characters[i++] = _commonCathode ? SPACE : ~SPACE;

	printx(milliseconds, characters);

	return sucess;
}

/*
PUBLIC
Displays text, left aligned in display, for the given milliseconds.
Accepted characters for string are A-Z, a-z, 0-9, -, space.
Returns true if displayed whole string
*/
bool ShiftDisplay::print(String text, int milliseconds) {
	bool sucess = true;
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
	if (text.length() > _nCharacters)
		sucess = false;

	// fill remaining right characters with empty
	while (i >= 0)
		characters[i--] = _commonCathode ? SPACE : ~SPACE;

	printx(milliseconds, characters);

	return sucess;
}

/*
PUBLIC
Displays character c left aligned in display, and integer value right aligned
in display, for the given milliseconds.
Returns true if displayed whole value.
*/
bool ShiftDisplay::printMenu(char c, int value, int milliseconds) {
	bool sucess = true;
	int negative = value < 0;
	byte characters[_nCharacters];
	int i = 0;

	// transform value in positive
	if (negative)
		value = value * -1;

	// get digits from number
	do {
		int digit = value % 10;
		characters[i++] = _commonCathode ? DIGITS[digit] : ~DIGITS[digit];
		value = value / 10;
	} while (value != 0 && i < _nCharacters - 1);
	if (value != 0)
		sucess = false;

	// place minus character at numbers left
	if (negative) {
		if (i < _nCharacters - 1)
			characters[i++] = _commonCathode ? MINUS : ~MINUS;
		else
			sucess = false;
	}

	// fill remaining characters with empty, except last
	while (i < _nCharacters - 1)
		characters[i++] = _commonCathode ? SPACE : ~SPACE;

	// place letter in front, with dot
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

	printx(milliseconds, characters);

	return sucess;
}

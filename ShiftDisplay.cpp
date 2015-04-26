#include "Arduino.h"
#include "ShiftDisplay.h"

const int POV = 5; // delay for persistence of vision

const byte EMPTY[2] = {
	B11111111, ~B11111111 // {}
};
const byte MINUS[2] = {
	B11110111, ~B11110111 // -
};
const byte DIGITS[10][2] = {
	{B10001000, ~B10001000}, // 0
	{B10111011, ~B10111011}, // 1
	{B11000001, ~B11000001}, // 2
	{B10010001, ~B10010001}, // 3
	{B10110010, ~B10110010}, // 4
	{B10010100, ~B10010100}, // 5
	{B10000100, ~B10000100}, // 6
	{B10111001, ~B10111001}, // 7
	{B10000000, ~B10000000}, // 8
	{B10110000, ~B10110000} // 9
};
const byte DECIMAL_POINT = B100000000;
const byte LETTERS[24][2] = {
	{B10000000, ~B10000000}, //
};
const byte DISPLAYS[8][2] = {
	{B00000001, ~B00000001},
	{B00000010, ~B00000010},
	{B00000100, ~B00000100},
	{B00001000, ~B00001000},
	{B00010000, ~B00010000},
	{B00100000, ~B00100000},
	{B01000000, ~B01000000},
	{B10000000, ~B10000000}
};
const byte DISPLAY_OFF[2] = {
	B00000000, ~B00000000
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

int ShiftDisplay::power(int number, int exponent) {
	return round(pow(number, exponent));
}

void ShiftDisplay::clear() {
	digitalWrite(_latchPin, LOW);
	for (int i = 0; i < _nShiftRegisters; i++)
		shiftOut(_dataPin, _clkPin, MSBFIRST, 0); // both ends of led with same value
	digitalWrite(_latchPin, HIGH);
}

void ShiftDisplay::printx(int milliseconds, byte characters[]) {
	unsigned long start = millis();
	while (millis() - start < milliseconds) {
		for (int i = 0; i < _nCharacters; i++) {
			digitalWrite(_latchPin, LOW);

			// shift data for all display registers
			for (int j = _nShiftRegisters - 2; j >= 0; j--) {
				byte out = DISPLAY_OFF[_commonCathode];
				if (i/8 == j)
					out = DISPLAYS[i][_commonCathode];
				shiftOut(_dataPin, _clkPin, MSBFIRST, out);
			}

			// shift data for number register
			shiftOut(_dataPin, _clkPin, MSBFIRST, characters[i]);

			digitalWrite(_latchPin, HIGH);
			delay(POV);
		}
	}
	clear();
}

// PUBLIC

bool ShiftDisplay::print(int number, int milliseconds) {
	bool sucess = true;
	int newNumber = number;
	byte characters[_nCharacters];
	int i = 0;

	// get digits from number
	do {
		characters[i++] = DIGITS[newNumber%10][_commonCathode];
		newNumber = newNumber / 10;
	} while (newNumber != 0 && i < _nCharacters); // at least one digit
	if (newNumber != 0)
		sucess = false;

	// place minus character at numbers left
	if (number < 0) {
		if (i < _nCharacters)
			characters[i++] = MINUS[_commonCathode];
		else
			sucess = false;
	}

	// fill remaining characters with empty
	while (i < _nCharacters)
		characters[i++] = EMPTY[_commonCathode];

	printx(milliseconds, characters);

	return sucess;
}

void ShiftDisplay::print(float number, int nDecimalPlaces, int milliseconds) {
	if (nDecimalPlaces == 0) {
		int n = round(number);
		return print(n, milliseconds);
	}

	bool sucess = true;
	int newNumber = round(number * power(10, nDecimalPlaces));
	byte characters[_nCharacters];
	int i = 0;

	// get digits from number
	do {
		characters[i++] = DIGITS[newNumber%10][_commonCathode];
		newNumber = newNumber / 10;
	} while (newNumber != 0 && i < _nCharacters);
	if (newNumber != 0)
		sucess = false;

	// place decimal point
	if (nDecimalPlaces < _nCharacters)
		characters[nDecimalPlaces] = characters[nDecimalPlaces] + DECIMAL_POINT;
	else
		sucess = false;

	// place minus character at numbers left
	if (number < 0) {
		if (i < _nCharacters)
			characters[i++] = MINUS[_commonCathode];
		else
			sucess = false;
	}

	// fill remaining characters with empty
	while (i < _nCharacters)
		characters[i++] = EMPTY[_commonCathode];

	printx(milliseconds, characters);

	return sucess;
}

void ShiftDisplay::print(String text, int milliseconds) {
	bool sucess = true;
	byte characters[_nCharacters];
	int i = 0;

	// get characters from text
	while (i < strlen(text) && i < _nCharacters)
		characters[i] = LETTERS[text[i++] - '0'][_commonCathode];
	if (strlen(text) > _nCharacters)
		sucess = false;

	// fill remaining characters with empty
	while (i < _nCharacters)
		characters[i++] = EMPTY[_commonCathode];

	printx(milliseconds, characters);

	return sucess;
}

void ShiftDisplay::printMenu(char title, int value, int milliseconds) {
	bool sucess = true;
	int value = value;
	byte characters[_nCharacters];
	int i = 0;

	// get digits from number
	do {
		characters[i++] = DIGITS[value%10][_commonCathode];
		value = value / 10;
	} while (value != 0 && i < _nCharacters - 1);
	if (value != 0)
		sucess = false;

	// place minus character at numbers left
	if (value < 0) {
		if (i < _nCharacters - 1)
			characters[i++] = MINUS[_commonCathode];
		else
			sucess = false;
	}

	// fill remaining characters with empty
	while (i < _nCharacters - 1)
		characters[i++] = EMPTY[_commonCathode];

	// place letter in front
	characters[i] = LETTERS[title - '0'][_commonCathode];

	printx(milliseconds, characters);

	return sucess;
}


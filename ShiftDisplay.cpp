#include "Arduino.h"
#include "ShiftDisplay.h"

const int POV = 5; // delay for persistence of vision

const byte EMPTY[2] = {
	~B00000000, B00000000 // {}
};
const byte MINUS[2] = {
	~B00001000, B00001000 // -
};
const byte DIGITSX[10][2] = {
	{~B01110111, B01110111}, // 0
	{~B01000100, B01000100}, // 1
	{~B00111110, B00111110}, // 2
	{~B01101110, B01101110}, // 3
	{~B01001101, B01001101}, // 4
	{~B01101011, B01101011}, // 5
	{~B01111011, B01111011}, // 6
	{~B01000110, B01000110}, // 7
	{~B01111111, B01111111}, // 8
	{~B01001111, B01001111} // 9
};
const byte DIGITS[10][2] = {
	{~B10000000,B10000000},
	{~B00000000,B11111111},
	{~B00000000,B00000000},
};
const byte DECIMAL_POINT = B10000000;
const byte LETTERS[26][2] = {
	{~B00000000, B00000000}, // space
	{~B10000000, B10000000}, //
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
	Serial.begin(9600); // TODO 
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

// PUBLIC print int
bool ShiftDisplay::print(int number, int milliseconds) {
	Serial.println("int");
	bool sucess = true;
	int newNumber = number;
	byte characters[_nCharacters];
	int i = 0;

	// get digits from number
	do {
		characters[i++] = DIGITS[newNumber%10][_commonCathode];
		Serial.println(newNumber%10); // bug if number negative, shit happens
		newNumber = newNumber / 10;
		Serial.println(newNumber);
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

// PUBLIC print float
bool ShiftDisplay::print(float number, int nDecimalPlaces, int milliseconds) {
	Serial.println("float");
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
	} while (newNumber != 0 && i < _nCharacters); // bug: 123.456, 10 so imprime um zero
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


// PUBLIC print string
bool ShiftDisplay::print(String text, int milliseconds) {
	Serial.println("string");
	bool sucess = true;
	byte characters[_nCharacters];
	int i = 0;

	// get characters from text
	while (i < text.length() && i < _nCharacters) {
		int c = text[i++];
		if (c >= 'A' && c <= 'Z')
			c = c - 'A';
		else if (c >= 'a' && c <= 'z')
			c = c - 'a';
		else
			c = 26;
		Serial.println(c);
		characters[i] = LETTERS[c][_commonCathode];
	}
	if (text.length() > _nCharacters)
		sucess = false;

	// fill remaining characters with empty
	while (i < _nCharacters)
		characters[i++] = EMPTY[_commonCathode];

	printx(milliseconds, characters);

	return sucess;
}

// PUBLIC print char and int
bool ShiftDisplay::printMenu(char title, int value, int milliseconds) {
	Serial.println("menu");
	bool sucess = true;
	int newValue = value;
	byte characters[_nCharacters];
	int i = 0;

	// get digits from number
	do {
		characters[i++] = DIGITS[newValue%10][_commonCathode];
		newValue = newValue / 10;
	} while (newValue != 0 && i < _nCharacters - 1);
	if (newValue != 0)
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
	int c = title;
	if (c >= 'A' && c <= 'Z')
		c = c - 'A';
	else if (c >= 'a' && c <= 'z')
		c = c - 'a';
	else
		c = 26;
	characters[i] = LETTERS[c][_commonCathode];

	printx(milliseconds, characters);

	return sucess;
}


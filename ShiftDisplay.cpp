#include "Arduino.h"
#include "ShiftDisplay.h"

const byte digits[10] = {
	B10001000, // 0
	B10111011, // 1
	B11000001, // 2
	B10010001, // 3
	B10110010, // 4
	B10010100, // 5
	B10000100, // 6
	B10111001, // 7
	B10000000, // 8
	B10110000 // 9
};

const byte symbols[1] = {
	B11110111, // -
};

const byte chars[3] = {
	B10000001, // a
	B10000110, // b
	B11000111, // c
};

const int POV = 5;


ShiftDisplay::ShiftDisplay(int latchPin, int clkPin, int dataPin, bool commonCathode, int nDigits) {
	pinMode(latchPin, OUTPUT);
	pinMode(clkPin, OUTPUT);
	pinMode(dataPin, OUTPUT);
	_latchPin = latchPin;
	_clkPin = clkPin;
	_dataPin = dataPin;
	_commonCathode = commonCathode;
	_nDigits = nDigits;
}

int ShiftDisplay::power(int number, int power) {
	return round(pow(number, power));
}

void ShiftDisplay::clear() {
	digitalWrite(_latchPin, LOW);
	int nShiftRegisters = _nDigits/8 + 2;
	for (int i = 0; i < nShiftRegisters; i++)
		shiftOut(_dataPin, _clkPin, MSBFIRST, 0); // both ends of led with same value
	digitalWrite(_latchPin, HIGH);
}

void ShiftDisplay::print(int number) {
	bool negative = false;
	if (number < 0) {
		negative = true;
		number = number * -1;
	}
	for (int pos = 0; pos < _nDigits; pos++) {
		byte display = power(2, pos);
		byte digit = digits[number%10]; // right digit of number
		if (_commonCathode) {
			display = ~display;
			digit = ~digit;
		}
		digitalWrite(_latchPin, LOW);
		shiftOut(_dataPin, _clkPin, MSBFIRST, display);
		shiftOut(_dataPin, _clkPin, MSBFIRST, digit);
		digitalWrite(_latchPin, HIGH);
		delay(POV);

		number = number / 10;
		if (number == 0)
			break;
	}
	if (negative) { // write minus to left
		byte display = power(2, pos+1); // TODO test if -1 or -999
		byte symbol = symbols[0];
		if (_commonCathode) {
			display = ~display;
			symbol = ~symbol;
		}
		digitalWrite(_latchPin, LOW);
		shiftOut(_dataPin, _clkPin, MSBFIRST, display);
		shiftOut(_dataPin, _clkPin, MSBFIRST, digit);
		digitalWrite(_latchPin, HIGH);
		delay(POV);
	}
}

// scroll
// leading zeros
// if number > 999

void ShiftDisplay::print(int number, int milliseconds) { // TODO if number < 10 ^ndigits -1
	unsigned long start = millis();
	while (millis() - start < milliseconds)
		print(number);
	clear();
}

void ShiftDisplay::print(float number, int milliseconds) {

}

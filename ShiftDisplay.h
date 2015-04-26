/*
	Pyntoo

	Multiple digit 7-segment display using shift registers

*/

#ifndef ShiftDisplay_h
#define ShiftDisplay_h
#include "Arduino.h"

class ShiftDisplay {
private:
	int _latchPin;
	int _clkPin;
	int _dataPin;
	bool _commonCathode;
	int _nCharacters;
	int _nShiftRegisters;
	int power(int number, int exponent);
	void clear();
	void printx(int milliseconds, byte characters[]);
public:
	ShiftDisplay(int latchPin, int clkPin, int dataPin, bool commonCathode, int _nDigits);
	bool print(int number, int milliseconds);
	bool print(float number, int nDecimalPlaces, int milliseconds);
	bool print(String text, int milliseconds);
	bool printMenu(char title, int value, int milliseconds);
};

#endif

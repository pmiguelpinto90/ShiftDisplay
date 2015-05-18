/*
  ShiftDisplay.h
  Library for driving multiple digit 7-segment displays using shift registers
  Created by Pyntoo, May 9, 2015
  Released into the public domain
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
	ShiftDisplay(int latchPin, int clkPin, int dataPin, bool commonCathode, int nDigits);
	bool print(int value, int milliseconds);
	bool print(float value, int nDecimalPlaces, int milliseconds);
	bool print(String text, int milliseconds);
	bool printMenu(char c, int value, int milliseconds);
};

#endif

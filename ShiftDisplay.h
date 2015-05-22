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
	int _clockPin;
	int _dataPin;
	int _outputEnablePin;
	bool _commonCathode;
	int _nCharacters;
	int _nShiftRegisters;
	int power(int number, int exponent);
	void clear();
	void printx(int milliseconds, byte characters[]);
public:
	ShiftDisplay(int latchPin, int clockPin, int dataPin, bool commonCathode, int nDigits);
	ShiftDisplay(int latchPin, int clockPin, int dataPin, int outputEnablePin, bool commonCathode, int nDigits);
	void print(int value, int milliseconds);
	void print(float value, int nDecimalPlaces, int milliseconds);
	void print(String text, int milliseconds);
	void printMenu(char c, int value, int milliseconds);
	void fadeIn(int value, int fadeTime, int milliseconds);
};

#endif

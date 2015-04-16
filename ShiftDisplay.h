/*
	Pyntoo

	Multiple digit 7-segment display using shift registers

*/

#ifndef ShiftDisplay_h
#define ShiftDisplay_h
#include "Arduino.h"

class ShiftDisplay {
public:
	ShiftDisplay(int latchPin, int clkPin, int dataPin, bool commonCathode, int nDigits);
	void print(int number, int milliseconds);
	void print(float number, int milliseconds);
	void scroll(int number, int speed);
	void scroll(float number, int speed);
private:
	int _latchPin;
	int _clkPin;
	int _dataPin;
	bool _commonCathode;
	int _nDigits;
	int power(int number, int power);
	int getDigit(int number, int pos);
	void clear();
	void print(int number);
};

#endif

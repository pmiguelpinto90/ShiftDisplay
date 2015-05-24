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
	void printx(int time, byte characters[]);
public:
	ShiftDisplay(int latchPin, int clockPin, int dataPin, bool commonCathode, int nDigits);
	ShiftDisplay(int latchPin, int clockPin, int dataPin, int outputEnablePin, bool commonCathode, int nDigits);
	void print(int value, int time);
	void print(float value, int time);
	void print(float value, int nDecimalPlaces, int time);
	void print(String text, int time);
	void printMenu(char c, int value, int time);
	void printMenu(char c, float value, int time);
	void printMenu(char c, float value, int nDecimalPlaces, int time);
	void printMenu(char c, String text, int time);
	void fadeIn(int value, int fadeTime, int time);
};

#endif

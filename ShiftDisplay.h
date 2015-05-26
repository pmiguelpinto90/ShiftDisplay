/*

  ShiftDisplay
  1.0.0 (26/05/2015)
  Arduino library for driving multiple digit 7-segment displays using shift registers
  
  Pyntoo
  r.pynto@gmail.com

  Released into the public domain on May 9, 2015

*/

#ifndef ShiftDisplay_h
#define ShiftDisplay_h
#include "Arduino.h"

class ShiftDisplay {
private:
	int _latchPin;
	int _clockPin;
	int _dataPin;
	// int _outputEnablePin;
	bool _commonCathode;
	int _nCharacters;
	int _nShiftRegisters;
	int power(int number, int exponent);
	void clear();
	void printx(byte characters[], int time); // int fade);
public:
	ShiftDisplay(int latchPin, int clockPin, int dataPin, bool commonCathode, int nDigits);
	// ShiftDisplay(int latchPin, int clockPin, int dataPin, int outputEnablePin, bool commonCathode, int nDigits);
	
	void print(int value, int time);
	void print(float value, int decimalPlaces, int time);
	void print(String text, int time);

	void printMenu(char c, int value, int time);
	void printMenu(char c, float value, int decimalPlaces, int time);
	void printMenu(char c, String text, int time);

	// void scroll(int value, bool invert, int time);
	// void scroll(float value, int decimalPlaces, bool invert, int time);
	// void scroll(String text, bool invert, int time);

	// void fade(int value, bool fadeout, int time);
	// void fade(float value, int decimalPlaces, bool fadeout, int time);
	// void fade(String text, bool fadeout, int time);
};

#endif

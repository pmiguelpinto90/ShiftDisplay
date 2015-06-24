/*
  ShiftDisplay 2.0.0 (24/06/2015)
  https://github.com/Pyntoo/ShiftDisplay
*/


#ifndef ShiftDisplayMini_h
#define ShiftDisplayMini_h
#include "Arduino.h"


class ShiftDisplayMini {

	private:
		int _latchPin;
		int _clockPin;
		int _dataPin;
		bool _commonCathode;
		int _displaySize;

		int power(int number, int exponent);
		void clear();
		void printx(byte characters[], int time);

	public:
		ShiftDisplayMini(int latchPin, int clockPin, int dataPin, bool commonCathode, int displaySize);
		
		void print(int value, int time);
		void print(float value, int decimalPlaces, int time);
		void print(String text, int time);
};

#endif

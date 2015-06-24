/*
  ShiftDisplay 2.0.0 (24/06/2015)
  https://github.com/Pyntoo/ShiftDisplay
*/


#ifndef ShiftDisplay_h
#define ShiftDisplay_h
#include "Arduino.h"


#define ANIMATION_NONE 0
#define ANIMATION_SCROLL 1
#define ANIMATION_FADEIN 2
#define ANIMATION_FADEOUT 3
#define ALIGNMENT_LEFT 1
#define ALIGNMENT_RIGHT 2
#define ALIGNMENT_CENTER 3


class ShiftDisplay {

	private:
		int _latchPin;
		int _clockPin;
		int _dataPin;
		int _outputEnablePin;
		bool _commonCathode;
		int _displaySize;
		int _nShiftRegisters;

		int power(int number, int exponent);
		int countDigits(long value);
		int countIntegerDigits(float value);
		int countCharacters(String text);

		void getDigits(long number, byte digits[], int nDigits);
		void getCharacters(String text, byte characters[], int nCharacters);
		void arrangeCharacters(byte characters[], int nCharacters, byte display[], int alignment);
		void arrangeCharactersScroll(byte characters[], int nCharacters, byte display[], int alignment, int step);
		
		void clear();
		void printDisplay(byte display[], int time, int animation);
		void printCharacters(byte characters[], int nCharacters, int time, int animation, int alignment);

	public:
		ShiftDisplay(int latchPin, int clockPin, int dataPin, bool commonCathode, int displaySize);
		ShiftDisplay(int latchPin, int clockPin, int dataPin, int outputEnablePin, bool commonCathode, int displaySize);
		
		void print(int value, int time, int animation, int alignment);
		void print(float value, int decimalPlaces, int time, int animation, int alignment);
		void print(String text, int time, int animation, int alignment);

		// void printMenu(char c, int value, int time);
		// void printMenu(char c, float value, int decimalPlaces, int time);
		// void printMenu(char c, String text, int time);
};

#endif

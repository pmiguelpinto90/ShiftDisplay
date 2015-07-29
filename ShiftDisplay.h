/*
  ShiftDisplay 2.2.0 (14/07/2015)
  https://github.com/Pyntoo/ShiftDisplay
*/


#ifndef ShiftDisplay_h
#define ShiftDisplay_h
#include "Arduino.h"


#define ALIGNMENT_LEFT 1
#define ALIGNMENT_RIGHT 2
#define ALIGNMENT_CENTER 3

#define ANIMATION_NONE 0
#define ANIMATION_EXIT_LEFT 1
#define ANIMATION_EXIT_RIGHT 2
#define ANIMATION_SCROLL 4
#define ANIMATION_FADE_IN 5
#define ANIMATION_FADE_OUT 6


class ShiftDisplay {

	private:
		int _latchPin;
		int _clockPin;
		int _dataPin;
		int _outputEnablePin;
		bool _commonCathode;
		int _displayLength;
		int _nShiftRegisters;

		int power(int number, int exponent);
		int countDigits(long value);
		int countIntegerDigits(float value);
		int countCharacters(String text);

		void getDigits(long number, byte digits[], int nDigits);
		void getCharacters(String text, byte characters[], int nCharacters);
		
		void clear();
		void showDisplay(byte display[], int time);
		void shiftDisplay(byte display[], bool toRight);
		void prepareDisplay(byte characters[], int nCharacters, byte display[], int alignment);
		void printCharacters(byte characters[], int nCharacters, int time, int animation, int alignment);

	public:
		ShiftDisplay(int latchPin, int clockPin, int dataPin, bool commonCathode, int displayLength);
		ShiftDisplay(int latchPin, int clockPin, int dataPin, int outputEnablePin, bool commonCathode, int displayLength);
		
		void print(int value);
		void print(int value, int time, int alignment, int animation);
		void print(float value, int decimalPlaces, int time, int alignment, int animation);
		void print(String text, int time, int alignment, int animation);

		// void printMenu(char c, int value, int time);
		// void printMenu(char c, float value, int decimalPlaces, int time);
		// void printMenu(char c, String text, int time);
};

#endif

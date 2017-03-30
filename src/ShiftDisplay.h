/*
ShiftDisplay
by MiguelPynto
Arduino library for driving multiple-digit 7-segment LED displays using 74HC595 shift registers
https://miguelpynto.github.io/ShiftDisplay/
*/

#ifndef ShiftDisplay_h
#define ShiftDisplay_h
#include "Arduino.h"

const char ALIGN_LEFT = 'L';
const char ALIGN_RIGHT = 'R';
const char ALIGN_CENTER = 'C';
const int COMMON_ANODE = 0;
const int COMMON_CATHODE = 1;
const int DEFAULT_LATCH_PIN = 6;
const int DEFAULT_CLOCK_PIN = 7;
const int DEFAULT_DATA_PIN = 5;
const int DEFAULT_DECIMAL_PLACES = 2;
const char DEFAULT_ALIGN_TEXT = ALIGN_LEFT;
const char DEFAULT_ALIGN_NUMBER = ALIGN_RIGHT;

class ShiftDisplay {

	private:
		int _latchPin;
		int _clockPin;
		int _dataPin;
		int _displayType;
		int _displaySize;
		byte _buffer[8]; // value set, encoded to print

		void construct(int latchPin, int clockPin, int dataPin, int displayType, int displaySize);
		int countCharacters(long number);
		int countCharacters(double number);
		void getCharacters(long input, char output[], int size);
		int formatCharacters(const char input[], int size, char output[], char alignment, int decimalPlaces);
		void encodeCharacters(const char input[], int pointIndex);
		void encodePoint(int index, bool show);
		void clearDisplay();
		void showDisplay();

	public:
		ShiftDisplay(int displayType, int displaySize);
		ShiftDisplay(int latchPin, int clockPin, int dataPin, int displayType, int displaySize);

		void set(int value, char alignment = DEFAULT_ALIGN_NUMBER);
		void set(long value, char alignment = DEFAULT_ALIGN_NUMBER);
		void set(double value, int decimalPlaces = DEFAULT_DECIMAL_PLACES, char alignment = DEFAULT_ALIGN_NUMBER);
		void set(double value, char alignment); // for overriding decimalPlaces obligation with alignment
		void set(char value, char alignment = DEFAULT_ALIGN_TEXT);
		void set(const char value[], char alignment = DEFAULT_ALIGN_TEXT);
		void set(const String &value, char alignment = DEFAULT_ALIGN_TEXT);
		void insertPoint(int index);
		void removePoint(int index);
		void show();
		void show(long time);
		void print(long time, int value, char alignment = DEFAULT_ALIGN_NUMBER);
		void print(long time, long value, char alignment = DEFAULT_ALIGN_NUMBER);
		void print(long time, double value, int decimalPlaces = DEFAULT_DECIMAL_PLACES, char alignment = DEFAULT_ALIGN_NUMBER);
		void print(long time, double value, char alignment); // for overriding decimalPlaces obligation with alignment
		void print(long time, char value, char alignment = DEFAULT_ALIGN_TEXT);
		void print(long time, const char value[], char alignment = DEFAULT_ALIGN_TEXT);
		void print(long time, const String &value, char alignment = DEFAULT_ALIGN_TEXT);
};

#endif

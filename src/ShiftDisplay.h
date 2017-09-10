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
const int MAX_DISPLAY_SIZE = 8;

class ShiftDisplay {

	private:
		int _latchPin;
		int _clockPin;
		int _dataPin;
		int _displayType;
		int _displayQuantity;
		int _displaySizes[MAX_DISPLAY_SIZE];
		int _displaySize;
		int _displayStarts[MAX_DISPLAY_SIZE];
		byte _buffer[MAX_DISPLAY_SIZE]; // value encoded to print

		void constructSingle(int latchPin, int clockPin, int dataPin, int displayType, int displaySize);
		void constructMultiple(int latchPin, int clockPin, int dataPin, int displayType, int displayQuantity, int displaySizes[]);
		int countCharacters(long number);
		int countCharacters(double number);
		void getCharacters(long input, int size, char output[]);
		int formatCharacters(int inSize, const char input[], int outSize, char output[], char alignment, int decimalPlaces);
		void encodeCharacters(int size, const char input[], byte output[], int dotIndex);
		void setBufferDot(int index, bool show);
		void setBuffer(int start, int size, byte input[]);
		void clearDisplay();
		void printDisplay();

	public:
		ShiftDisplay(int displayType, int displaySize);
		ShiftDisplay(int latchPin, int clockPin, int dataPin, int displayType, int displaySize);
		ShiftDisplay(int displayType, int displayQuantity, int displaySizes[]);
		ShiftDisplay(int latchPin, int clockPin, int dataPin, int displayType, int displayQuantity, int displaySizes[]);
		void set(int value, char alignment = DEFAULT_ALIGN_NUMBER);
		void set(long value, char alignment = DEFAULT_ALIGN_NUMBER);
		void set(double value, int decimalPlaces = DEFAULT_DECIMAL_PLACES, char alignment = DEFAULT_ALIGN_NUMBER);
		void set(double value, char alignment); // for overriding decimalPlaces obligation with alignment
		void set(char value, char alignment = DEFAULT_ALIGN_TEXT);
		void set(const char value[], char alignment = DEFAULT_ALIGN_TEXT);
		void set(const String &value, char alignment = DEFAULT_ALIGN_TEXT);
		void set(const byte codes[]);
		void set(const char characters[], bool dots[]);
		void setAt(int displayId, int value, char alignment = DEFAULT_ALIGN_NUMBER);
		void setAt(int displayId, long value, char alignment = DEFAULT_ALIGN_NUMBER);
		void insertDot(int index);
		void removeDot(int index);
		void show();
		void show(unsigned long time);
		void show(int value, unsigned long time, char alignment = DEFAULT_ALIGN_NUMBER);
		void show(long value, unsigned long time, char alignment = DEFAULT_ALIGN_NUMBER);
		void show(double value, unsigned long time, int decimalPlaces = DEFAULT_DECIMAL_PLACES, char alignment = DEFAULT_ALIGN_NUMBER);
		void show(double value, unsigned long time, char alignment); // for overriding decimalPlaces obligation in above function
		void show(char value, unsigned long time, char alignment = DEFAULT_ALIGN_TEXT);
		void show(const char value[], unsigned long time, char alignment = DEFAULT_ALIGN_TEXT);
		void show(const String &value, unsigned long time, char alignment = DEFAULT_ALIGN_TEXT);
		void show(const byte codes[], unsigned long time);
		void show(const char characters[], bool dots[], unsigned long time);

	// compatibility:
		void insertPoint(int index); // insertDot
		void removePoint(int index); // removeDot
		void print(long time, int value, char alignment = DEFAULT_ALIGN_NUMBER); // show
		void print(long time, long value, char alignment = DEFAULT_ALIGN_NUMBER); // show
		void print(long time, double value, int decimalPlaces = DEFAULT_DECIMAL_PLACES, char alignment = DEFAULT_ALIGN_NUMBER); // show
		void print(long time, double value, char alignment); // show
		void print(long time, char value, char alignment = DEFAULT_ALIGN_TEXT); // show
		void print(long time, const char value[], char alignment = DEFAULT_ALIGN_TEXT); // show
		void print(long time, const String &value, char alignment = DEFAULT_ALIGN_TEXT); // show

};

#endif

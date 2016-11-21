// github.com/MiguelPynto/ShiftDisplay


#ifndef ShiftDisplay_h
#define ShiftDisplay_h
#include "Arduino.h"


const int ALIGN_LEFT = 0;
const int ALIGN_RIGHT = 1;
const int ALIGN_CENTER = 2;

const int COMMON_ANODE = 0;
const int COMMON_CATHODE = 1;

const int DEFAULT_LATCH_PIN = 6;
const int DEFAULT_CLOCK_PIN = 7;
const int DEFAULT_DATA_PIN = 5;

const int DEFAULT_DECIMAL_PLACES = 2;
const int DEFAULT_ALIGN_TEXT = ALIGN_RIGHT;
const int DEFAULT_ALIGN_NUMBER = ALIGN_LEFT;

const int POV_TOTAL_TIME = 10; // time (in milliseconds) to complete one display iteration, achieving persistence of vision


class ShiftDisplay {

	private:
		int _latchPin;
		int _clockPin;
		int _dataPin;
		int _displayType;
		int _displaySize;
		int _povIndexTime;
		byte _buffer[8]; // value set, encoded to print

		void construct(int latchPin, int clockPin, int dataPin, int displayType, int displaySize);
		int countCharacters(long number);
		int countCharacters(double number);
		void getCharacters(long input, char output[], int size);
		int formatCharacters(const char input[], int size, char output[], int alignment, int decimalPlaces);
		void encodeCharacters(const char input[], int pointPosition);
		void clearDisplay();
		void showDisplay();

	public:
		ShiftDisplay(int displayType, int displaySize);
		ShiftDisplay(int latchPin, int clockPin, int dataPin, int displayType, int displaySize);

		//void begin(int displayType, int displaySize);
		void set(int value, int alignment = DEFAULT_ALIGN_NUMBER);
		void set(long value, int alignment = DEFAULT_ALIGN_NUMBER);
		void set(double value, int decimalPlaces = DEFAULT_DECIMAL_PLACES, int alignment = DEFAULT_ALIGN_NUMBER);
		void set(char value, int alignment = DEFAULT_ALIGN_TEXT);
		void set(const char value[], int alignment = DEFAULT_ALIGN_TEXT);
		void set(const String &value, int alignment = DEFAULT_ALIGN_TEXT);
		void show();
		void show(long time);
		void print(long time, int value, int alignment = DEFAULT_ALIGN_NUMBER);
		void print(long time, long value, int alignment = DEFAULT_ALIGN_NUMBER);
		void print(long time, double value, int decimalPlaces = DEFAULT_DECIMAL_PLACES, int alignment = DEFAULT_ALIGN_NUMBER);
		void print(long time, char value, int alignment = DEFAULT_ALIGN_TEXT);
		void print(long time, const char value[], int alignment = DEFAULT_ALIGN_TEXT);
		void print(long time, const String &value, int alignment = DEFAULT_ALIGN_TEXT);
};


#endif

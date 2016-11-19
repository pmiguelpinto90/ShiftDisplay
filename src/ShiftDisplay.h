// github.com/MiguelPynto/ShiftDisplay


#ifndef ShiftDisplay_h
#define ShiftDisplay_h
#include "Arduino.h"


#define ALIGN_LEFT 0
#define ALIGN_RIGHT 1
#define ALIGN_CENTER 2
#define COMMON_ANODE 0
#define COMMON_CATHODE 1


class ShiftDisplay {

	private:
		int _latchPin;
		int _clockPin;
		int _dataPin;
		int _displayType;
		int _displaySize;
		byte _buffer[8]; // value set by user, encoded to print
		int _povDelay;

		int countCharacters(long number);
		int countCharacters(double number);
		int countCharacters(const char text[]);
		void getCharacters(long input, char output[], int size);
		void formatCharacters(const char input[], int size, char output[], int alignment);
		int formatCharacters(const char input[], int size, char output[], int alignment, int decimalPlaces);
		void encodeCharacters(const char input[]);
		void encodeCharacters(const char input[], int pointPosition);
		void clearDisplay();
		void showDisplay();

	public:
		ShiftDisplay(int latchPin, int clockPin, int dataPin, int displayType, int displayLength);

		void set(int value, int alignment = ALIGN_RIGHT);
		void set(long value, int alignment = ALIGN_RIGHT);
		void set(double value, int decimalPlaces = 2, int alignment = ALIGN_RIGHT);
		void set(char value, int alignment = ALIGN_CENTER);
		void set(const char value[], int alignment = ALIGN_LEFT);
		void set(const String &value, int alignment = ALIGN_LEFT);
		void show();
		void show(long time);
		void print(long time, int value, int alignment = ALIGN_RIGHT);
		void print(long time, long value, int alignment = ALIGN_RIGHT);
		void print(long time, double value, int decimalPlaces = 2, int alignment = ALIGN_RIGHT);
		void print(long time, char value, int alignment = ALIGN_CENTER);
		void print(long time, const char value[], int alignment = ALIGN_LEFT);
		void print(long time, const String &value, int alignment = ALIGN_LEFT);
};


#endif

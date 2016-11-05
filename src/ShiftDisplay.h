// github.com/MiguelPynto/ShiftDisplay


#ifndef ShiftDisplay_h
#define ShiftDisplay_h
#include "Arduino.h"


#define ALIGN_LEFT 0
#define ALIGN_RIGHT 1
#define ALIGN_CENTER 2


class ShiftDisplay {

	private:
		int _latchPin;
		int _clockPin;
		int _dataPin;
		bool _commonCathode;
		int _displaySize;
		byte _display[8]; // value set by user, encoded to print
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
		void printDisplay();

	public:
		ShiftDisplay(int latchPin, int clockPin, int dataPin, bool commonCathode, int displayLength);

		void set(int number, int alignment = ALIGN_RIGHT);
		void set(long number, int alignment = ALIGN_RIGHT);
		void set(double number, int decimalPlaces = 2, int alignment = ALIGN_RIGHT);
		void set(char letter, int alignment = ALIGN_CENTER);
		void set(const char text[], int alignment = ALIGN_LEFT);
		void set(const String &text, int alignment = ALIGN_LEFT);
		void refresh();
		void show(int time);
};


#endif

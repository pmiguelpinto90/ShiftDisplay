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
		int countCharacters(char text[]);
		void getCharacters(long input, char output[], int size);
		void getCharacters(char input[], char output[], int size);
		void formatCharacters(char input[], int size, char output[], int alignment);
		int formatCharacters(char input[], int size, char output[], int alignment, int decimalPlaces);
		void encodeCharacters(char input[]);
		void encodeCharacters(char input[], int pointPosition);
		void clearDisplay();
		void printDisplay();

	public:
		ShiftDisplay(int latchPin, int clockPin, int dataPin, bool commonCathode, int displayLength);

		void set(int number, int alignment = ALIGN_RIGHT);
		void set(long number, int alignment = ALIGN_RIGHT);
		void set(float number, int decimalPlaces = 2, int alignment = ALIGN_RIGHT);
		void set(double number, int decimalPlaces = 2, int alignment = ALIGN_RIGHT);
		void set(char text[], int alignment = ALIGN_LEFT);
		void set(String text, int alignment = ALIGN_LEFT);
		void refresh();
		void show(int time);
};


#endif

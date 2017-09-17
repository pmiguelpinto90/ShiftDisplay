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
const int POV = 1; // milliseconds showing each character when iterating

class ShiftDisplay {

	private:

		int _latchPin;
		int _clockPin;
		int _dataPin;
		int _displayType;
		int _displaySize; // length of whole display
		int _sectionCount; // quantity of display sections
		int _sectionSizes[MAX_DISPLAY_SIZE]; // length of each section
		int _sectionBegins[MAX_DISPLAY_SIZE]; // index where each section begins on whole display
		byte _buffer[MAX_DISPLAY_SIZE]; // value to show on display (encoded in abcdefgp format)

		void initPins(int latchPin, int clockPin, int dataPin); // initialize shift register pins and clears it
		void constructSingleDisplay(int latchPin, int clockPin, int dataPin, int displayType, int displaySize); // common instructions to be called by single display constructors
		void constructSectionedDisplay(int latchPin, int clockPin, int dataPin, int displayType, int sectionCount, int sectionSizes[]); // common instructions to be called by sectioned display constructors

		void showDisplay(); // iterate buffer value on each display index, achieving persistence of vision
		void clearDisplay(); // clear shift registers
		void modifyBuffer(int index, byte code); // change buffer content in a single position
		void modifyBuffer(int beginIndex, int size, byte codes[]); // change buffer content in defined interval
		void modifyBufferDot(int index, bool dot); // change buffer dot in a single position
		void encodeCharacters(int size, const char input[], byte output[], int dotIndex); // encode array of chars to array of bytes in abcdefgp format
		int formatCharacters(int inSize, const char input[], int outSize, char output[], char alignment, int decimalPlaces); // arrange array of chars for displaying in specified alignment, returns dot index on display or -1 if none
		void getCharacters(long input, int size, char output[]); // convert an integer number to an array of chars
		int countCharacters(long number); // calculate the length of an array of chars for an integer number
		int countCharacters(double number); // calculate the length of an array of chars for the integer part on a real number

	public:

		// constructors
		ShiftDisplay(int displayType, int displaySize); // default pins
		ShiftDisplay(int latchPin, int clockPin, int dataPin, int displayType, int displaySize); // custom pins
		ShiftDisplay(int displayType, int sectionCount, int sectionSizes[]); // default pins, sectioned display
		ShiftDisplay(int latchPin, int clockPin, int dataPin, int displayType, int sectionCount, int sectionSizes[]); // custom pins, sectioned display

		// store a value to buffer
		void set(int value, char alignment = DEFAULT_ALIGN_NUMBER);
		void set(long value, char alignment = DEFAULT_ALIGN_NUMBER);
		void set(double valueReal, int decimalPlaces = DEFAULT_DECIMAL_PLACES, char alignment = DEFAULT_ALIGN_NUMBER);
		void set(double valueReal, char alignment); // override decimalPlaces obligation in function above
		void set(char value, char alignment = DEFAULT_ALIGN_TEXT);
		void set(const char value[], char alignment = DEFAULT_ALIGN_TEXT); // c string
		void set(const String &value, char alignment = DEFAULT_ALIGN_TEXT); // Arduino string object
		void set(const byte customs[]); // custom characters (encoded in abcdefgp format), array length must match display size
		void set(const char characters[], bool dots[]); // arrays length must match display size

		// modify buffer at index
		void setDot(int index, bool dot); // show or hide a dot
		void setCustom(int index, byte custom); // replace with a custom character (encoded in abcdefgp format)

		// store a value to buffer in section indexes
		void setAt(int section, int value, char alignment = DEFAULT_ALIGN_NUMBER);
		void setAt(int section, long value, char alignment = DEFAULT_ALIGN_NUMBER);
		void setAt(int section, double valueReal, int decimalPlaces = DEFAULT_DECIMAL_PLACES, char alignment = DEFAULT_ALIGN_NUMBER);
		void setAt(int section, double valueReal, char alignment); // override decimalPlaces obligation in function above
		void setAt(int section, char value, char alignment = DEFAULT_ALIGN_TEXT);
		void setAt(int section, const char value[], char alignment = DEFAULT_ALIGN_TEXT); // c string
		void setAt(int section, const String &value, char alignment = DEFAULT_ALIGN_TEXT); // Arduino string object
		void setAt(int section, const byte customs[]); // custom characters (encoded in abcdefgp format), array length must match defined section size
		void setAt(int section, const char characters[], bool dots[]); // arrays length must match defined section size

		// modify buffer at index in section
		void setDotAt(int section, int relativeIndex, bool dot); // show or hide a dot
		void setCustomAt(int section, int relativeIndex, byte custom); // replace with a custom character (encoded in abcdefgp format)

		// show buffer value on display
		void show(); // for a single iteration
		void show(unsigned long time); // for the specified time (or less if would exceed it)

		// store a value to buffer and show it on display for the specified time (or less if would exceed it)
		void show(int value, unsigned long time, char alignment = DEFAULT_ALIGN_NUMBER);
		void show(long value, unsigned long time, char alignment = DEFAULT_ALIGN_NUMBER);
		void show(double valueReal, unsigned long time, int decimalPlaces = DEFAULT_DECIMAL_PLACES, char alignment = DEFAULT_ALIGN_NUMBER);
		void show(double valueReal, unsigned long time, char alignment); // override decimalPlaces obligation in function above
		void show(char value, unsigned long time, char alignment = DEFAULT_ALIGN_TEXT);
		void show(const char value[], unsigned long time, char alignment = DEFAULT_ALIGN_TEXT); // c string
		void show(const String &value, unsigned long time, char alignment = DEFAULT_ALIGN_TEXT); // Arduino string object
		void show(const byte customs[], unsigned long time); // custom characters (encoded in abcdefgp format), array length must match display size
		void show(const char characters[], bool dots[], unsigned long time); // arrays length must match display size

		// duplicates to retain compatibility with old versions
		void insertPoint(int index); // deprecated by setDot()
		void removePoint(int index); // deprecated by setDot()
		void insertDot(int index); // deprecated by setDot()
		void removeDot(int index); // deprecated by setDot()
		void print(long time, int value, char alignment = DEFAULT_ALIGN_NUMBER); // deprecated by show()
		void print(long time, long value, char alignment = DEFAULT_ALIGN_NUMBER); // deprecated by show()
		void print(long time, double value, int decimalPlaces = DEFAULT_DECIMAL_PLACES, char alignment = DEFAULT_ALIGN_NUMBER); // deprecated by show()
		void print(long time, double value, char alignment); // deprecated by show()
		void print(long time, char value, char alignment = DEFAULT_ALIGN_TEXT); // deprecated by show()
		void print(long time, const char value[], char alignment = DEFAULT_ALIGN_TEXT); // deprecated by show()
		void print(long time, const String &value, char alignment = DEFAULT_ALIGN_TEXT); // deprecated by show()
};

#endif

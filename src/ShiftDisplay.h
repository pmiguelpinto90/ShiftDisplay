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

const int DEFAULT_LATCH_PIN = 6;
const int DEFAULT_CLOCK_PIN = 7;
const int DEFAULT_DATA_PIN = 5;
const int DEFAULT_DECIMAL_PLACES = 1;
const char DEFAULT_ALIGN_TEXT = ALIGN_LEFT;
const char DEFAULT_ALIGN_NUMBER = ALIGN_RIGHT;

const int MAX_DISPLAY_SIZE = 8;
const int POV = 1; // milliseconds showing each character when iterating

enum DisplayType {
	COMMON_ANODE,
	COMMON_CATHODE,
	INDIVIDUAL_ANODE,
	INDIVIDUAL_CATHODE,
};

class ShiftDisplay {

	// CP: for common pin type display
	// IP: for individual pin type display

	private:

		int _latchPin;
		int _clockPin;
		int _dataPin;
		bool _isCathode; // display type is common cathode or individual cathode
		bool _isMultiplex; // display type is common cathode or common anode
		int _displaySize; // length of whole display
		int _sectionCount; // quantity of display sections
		int _sectionSizes[MAX_DISPLAY_SIZE]; // length of each section
		int _sectionBegins[MAX_DISPLAY_SIZE]; // index where each section begins on whole display
		byte _cache[MAX_DISPLAY_SIZE]; // value to show on display (encoded in abcdefgp format)

		void initPins(int latchPin, int clockPin, int dataPin); // initialize shift register pins and clears it
		void construct(int latchPin, int clockPin, int dataPin, DisplayType displayType, int sectionCount, const int sectionSizes[]); // common instructions to be called by constructors

		void showMultiplexDisplay(); // CP: iterate stored value on each display index, achieving persistence of vision
		void showConstantDisplay(); // IP: send stored value to whole display
		void clearMultiplexDisplay(); // CP: clear both shift registers
		void clearConstantDisplay(); // IP: clear all shift registers

		void modifyCache(int index, byte code); // replace a position in cache
		void modifyCache(int beginIndex, int size, const byte codes[]); // replace a interval in cache
		void modifyCacheDot(int index, bool dot); // change dot in a cache position

		void encodeCharacters(int size, const char input[], byte output[], int dotIndex); // encode array of chars to array of bytes in abcdefgp format
		int formatCharacters(int inSize, const char input[], int outSize, char output[], char alignment, int decimalPlaces); // arrange array of chars for displaying in specified alignment, returns dot index on display or -1 if none
		void getCharacters(long input, int size, char output[]); // convert an integer number to an array of chars
		int countCharacters(long number); // calculate the length of an array of chars for an integer number
		int countCharacters(double number); // calculate the length of an array of chars for the integer part on a real number

	public:

		// constructors
		ShiftDisplay(DisplayType displayType, int displaySize); // default pins
		ShiftDisplay(int latchPin, int clockPin, int dataPin, DisplayType displayType, int displaySize); // custom pins
		ShiftDisplay(DisplayType displayType, int sectionCount, const int sectionSizes[]); // default pins, sectioned display
		ShiftDisplay(int latchPin, int clockPin, int dataPin, DisplayType displayType, int sectionCount, const int sectionSizes[]); // custom pins, sectioned display

		// cache value
		void set(int value, char alignment = DEFAULT_ALIGN_NUMBER);
		void set(long value, char alignment = DEFAULT_ALIGN_NUMBER);
		void set(double valueReal, int decimalPlaces = DEFAULT_DECIMAL_PLACES, char alignment = DEFAULT_ALIGN_NUMBER);
		void set(double valueReal, char alignment); // override decimalPlaces obligation in function above
		void set(char value, char alignment = DEFAULT_ALIGN_TEXT);
		void set(const char value[], char alignment = DEFAULT_ALIGN_TEXT); // c string
		void set(const String &value, char alignment = DEFAULT_ALIGN_TEXT); // Arduino string object
		void set(const byte customs[]); // custom characters (encoded in abcdefgp format), array length must match display size
		void set(const char characters[], const bool dots[]); // arrays length must match display size

		// modify cached value at index
		void setDot(int index, bool dot); // show or hide a dot on character
		void setCustom(int index, byte custom); // replace with a custom character (encoded in abcdefgp format)

		// cache value at section indexes
		void setAt(int section, int value, char alignment = DEFAULT_ALIGN_NUMBER);
		void setAt(int section, long value, char alignment = DEFAULT_ALIGN_NUMBER);
		void setAt(int section, double valueReal, int decimalPlaces = DEFAULT_DECIMAL_PLACES, char alignment = DEFAULT_ALIGN_NUMBER);
		void setAt(int section, double valueReal, char alignment); // override decimalPlaces obligation in function above
		void setAt(int section, char value, char alignment = DEFAULT_ALIGN_TEXT);
		void setAt(int section, const char value[], char alignment = DEFAULT_ALIGN_TEXT); // c string
		void setAt(int section, const String &value, char alignment = DEFAULT_ALIGN_TEXT); // Arduino string object
		void setAt(int section, const byte customs[]); // custom characters (encoded in abcdefgp format), array length must match defined section size
		void setAt(int section, const char characters[], const bool dots[]); // arrays length must match defined section size

		// modify cached value at index in section
		void setDotAt(int section, int relativeIndex, bool dot); // show or hide a dot on character
		void setCustomAt(int section, int relativeIndex, byte custom); // replace with a custom character (encoded in abcdefgp format)

		// IP: hide display content
		void hide();

		// show cached value on display
		void show(); // CP: for a single iteration; IP: while not hide or recalled
		void show(unsigned long time); // for the specified time (or less if would exceed it)

		// cache and show value on display for the specified time (or less if would exceed it)
		void show(int value, unsigned long time, char alignment = DEFAULT_ALIGN_NUMBER);
		void show(long value, unsigned long time, char alignment = DEFAULT_ALIGN_NUMBER);
		void show(double valueReal, unsigned long time, int decimalPlaces = DEFAULT_DECIMAL_PLACES, char alignment = DEFAULT_ALIGN_NUMBER);
		void show(double valueReal, unsigned long time, char alignment); // override decimalPlaces obligation in function above
		void show(char value, unsigned long time, char alignment = DEFAULT_ALIGN_TEXT);
		void show(const char value[], unsigned long time, char alignment = DEFAULT_ALIGN_TEXT); // c string
		void show(const String &value, unsigned long time, char alignment = DEFAULT_ALIGN_TEXT); // Arduino string object
		void show(const byte customs[], unsigned long time); // custom characters (encoded in abcdefgp format), array length must match display size
		void show(const char characters[], const bool dots[], unsigned long time); // arrays length must match display size

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

/*
ShiftDisplay
by MiguelPynto
Arduino library for driving 7-segment displays using shift registers
https://miguelpynto.github.io/ShiftDisplay/
*/

#ifndef ShiftDisplay_h
#define ShiftDisplay_h
#include "Arduino.h"

enum DisplayType {
	COMMON_ANODE,
	COMMON_CATHODE
};
enum DisplayDrive {
	MULTIPLEXED_DRIVE,
	STATIC_DRIVE
};

const char ALIGN_LEFT = 'L';
const char ALIGN_RIGHT = 'R';
const char ALIGN_CENTER = 'C';

const int DEFAULT_LATCH_PIN = 6;
const int DEFAULT_CLOCK_PIN = 7;
const int DEFAULT_DATA_PIN = 5;
const int DEFAULT_DECIMAL_PLACES = 1;
const char DEFAULT_ALIGN_TEXT = ALIGN_LEFT;
const char DEFAULT_ALIGN_NUMBER = ALIGN_RIGHT;
const bool DEFAULT_SET_DOT = true;
const bool DEFAULT_LEADING_ZEROS = false;

const int MAX_DISPLAY_SIZE = 8;
const int POV = 1; // milliseconds showing each character when iterating

class ShiftDisplay {

	// MD: for multiplexed drive displays
	// SD: for static drive displays

	private:

		int _latchPin;
		int _clockPin;
		int _dataPin;
		bool _isCathode;
		bool _isMultiplexed;
		int _displaySize; // length of whole display
		int _sectionCount; // quantity of display sections
		int _sectionSizes[MAX_DISPLAY_SIZE]; // length of each section
		int _sectionBegins[MAX_DISPLAY_SIZE]; // index where each section begins on whole display
		byte _cache[MAX_DISPLAY_SIZE]; // value to show on display (encoded in abcdefgp format)

		void construct(int latchPin, int clockPin, int dataPin, DisplayType displayType, int sectionCount, const int sectionSizes[], DisplayDrive displayDrive); // common instructions to be called by constructors

		void updateMultiplexedDisplay(); // MD: iterate stored value on each display index, achieving persistence of vision
		void updateStaticDisplay(); // SD: send stored value to whole display
		void clearMultiplexedDisplay(); // MD: clear both shift registers
		void clearStaticDisplay(); // SD: clear all shift registers

		void modifyCache(int index, byte code); // replace a position in cache
		void modifyCache(int beginIndex, int size, const byte codes[]); // replace a interval in cache
		void modifyCacheDot(int index, bool dot); // change dot in a cache position

		void encodeCharacters(int size, const char input[], byte output[], int dotIndex); // encode array of chars to array of bytes in abcdefgp format
		int formatCharacters(int inSize, const char input[], int outSize, char output[], char alignment, bool leadingZeros, int decimalPlaces); // arrange array of chars for displaying in specified alignment, returns dot index on display or -1 if none
		void getCharacters(long input, int size, char output[]); // convert an integer number to an array of chars
		int countCharacters(long number); // calculate the length of an array of chars for an integer number
		int countCharacters(double number); // calculate the length of an array of chars for the integer part on a real number
		
		void setInteger(long value, bool leadingZeros, char alignment, int section);
		void setReal(double value, int decimalPlaces, bool leadingZeros, char alignment, int section);
		void setChar(char value, char alignment, int section);
		void setCharArray(const char value[], char alignment, int section);
		void setString(const String &value, char alignment, int section);

		bool isValidSection(int section);

	public:

		// constructors
		ShiftDisplay(DisplayType displayType, int displaySize, DisplayDrive displayDrive = MULTIPLEXED_DRIVE); // default pins
		ShiftDisplay(int latchPin, int clockPin, int dataPin, DisplayType displayType, int displaySize, DisplayDrive displayDrive = MULTIPLEXED_DRIVE); // custom pins
		ShiftDisplay(DisplayType displayType, int sectionCount, const int sectionSizes[], DisplayDrive displayDrive = MULTIPLEXED_DRIVE); // default pins, sectioned display
		ShiftDisplay(int latchPin, int clockPin, int dataPin, DisplayType displayType, int sectionCount, const int sectionSizes[], DisplayDrive displayDrive = MULTIPLEXED_DRIVE); // custom pins, sectioned display

		// cache value
		void set(int value, bool leadingZeros = DEFAULT_LEADING_ZEROS, char alignment = DEFAULT_ALIGN_NUMBER);
		void set(int value, char alignment); // override leadingZeros obligation
		void set(long value, bool leadingZeros = DEFAULT_LEADING_ZEROS, char alignment = DEFAULT_ALIGN_NUMBER);
		void set(long value, char alignment); // override leadingZeros obligation
		void set(double valueReal, int decimalPlaces = DEFAULT_DECIMAL_PLACES, bool leadingZeros = DEFAULT_LEADING_ZEROS, char alignment = DEFAULT_ALIGN_NUMBER);
		void set(double valueReal, char alignment); // override decimalPlaces and leadingZeros obligation
		void set(char value, char alignment = DEFAULT_ALIGN_TEXT);
		void set(const char value[], char alignment = DEFAULT_ALIGN_TEXT); // c string
		void set(const String &value, char alignment = DEFAULT_ALIGN_TEXT); // Arduino string object
		void set(const byte customs[]); // custom characters (encoded in abcdefgp format), array length must match display size
		void set(const char characters[], const bool dots[]); // arrays length must match display size
		void setAt(int section, int value, bool leadingZeros = DEFAULT_LEADING_ZEROS, char alignment = DEFAULT_ALIGN_NUMBER);
		void setAt(int section, int value, char alignment); // override leadingZeros obligation
		void setAt(int section, long value, bool leadingZeros = DEFAULT_LEADING_ZEROS, char alignment = DEFAULT_ALIGN_NUMBER);
		void setAt(int section, long value, char alignment); // override leadingZeros obligation
		void setAt(int section, double valueReal, int decimalPlaces = DEFAULT_DECIMAL_PLACES, bool leadingZeros = DEFAULT_LEADING_ZEROS, char alignment = DEFAULT_ALIGN_NUMBER);
		void setAt(int section, double valueReal, char alignment); // override decimalPlaces obligation
		void setAt(int section, char value, char alignment = DEFAULT_ALIGN_TEXT);
		void setAt(int section, const char value[], char alignment = DEFAULT_ALIGN_TEXT); // c string
		void setAt(int section, const String &value, char alignment = DEFAULT_ALIGN_TEXT); // Arduino string object
		void setAt(int section, const byte customs[]); // custom characters (encoded in abcdefgp format), array length must match defined section size
		void setAt(int section, const char characters[], const bool dots[]); // arrays length must match defined section size

		// modify cached value at index
		void setDot(int index, bool dot = DEFAULT_SET_DOT); // show or hide a dot on character
		void setCustom(int index, byte custom); // replace with a custom character (encoded in abcdefgp format)
		void setDotAt(int section, int relativeIndex, bool dot = DEFAULT_SET_DOT); // show or hide a dot on character
		void setCustomAt(int section, int relativeIndex, byte custom); // replace with a custom character (encoded in abcdefgp format)

		// show cached value on display
		void update(); // MD: for a single iteration; SD: while not update/clear/show called

		// clear display content
		void clear();

		// show cached value on display for the specified time (or less if would exceed it)
		void show(unsigned long time);

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
		void show(); // deprecated by update()
		void show(int value, unsigned long time, char alignment = DEFAULT_ALIGN_NUMBER); // deprecated by set() show()
		void show(long value, unsigned long time, char alignment = DEFAULT_ALIGN_NUMBER); // deprecated by set() show()
		void show(double valueReal, unsigned long time, int decimalPlaces = DEFAULT_DECIMAL_PLACES, char alignment = DEFAULT_ALIGN_NUMBER); // deprecated by set() show()
		void show(double valueReal, unsigned long time, char alignment); // deprecated by set() show()
		void show(char value, unsigned long time, char alignment = DEFAULT_ALIGN_TEXT); // deprecated by set() show()
		void show(const char value[], unsigned long time, char alignment = DEFAULT_ALIGN_TEXT); // deprecated by set() show()
		void show(const String &value, unsigned long time, char alignment = DEFAULT_ALIGN_TEXT); // deprecated by set() show()
		void show(const byte customs[], unsigned long time); // deprecated by set() show()
		void show(const char characters[], const bool dots[], unsigned long time); // deprecated by set() show()
};

#endif

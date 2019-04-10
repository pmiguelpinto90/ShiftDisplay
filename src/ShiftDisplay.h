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
enum Alignment {
	ALIGN_LEFT = 'L',
	ALIGN_RIGHT = 'R',
	ALIGN_CENTER = 'C'
};

const int DEFAULT_LATCH_PIN = 6;
const int DEFAULT_CLOCK_PIN = 7;
const int DEFAULT_DATA_PIN = 5;
const Alignment DEFAULT_ALIGN_TEXT = ALIGN_LEFT;
const Alignment DEFAULT_ALIGN_NUMBER = ALIGN_RIGHT;
const int DEFAULT_DECIMAL_PLACES_REAL = 1;
const int DEFAULT_DECIMAL_PLACES_INTEGER = 0;
const bool DEFAULT_LEADING_ZEROS = false;
const bool DEFAULT_SWAPPED_REGISTERS = false;
const bool DEFAULT_CHANGE_DOT = true;
const DisplayDrive DEFAULT_DRIVE = MULTIPLEXED_DRIVE;
const int DEFAULT_INDEXES[] = { 0, 1, 2, 3, 4, 5, 6, 7 };

const int MAX_DISPLAY_SIZE = 8;
const int POV = 1; // milliseconds showing each character when multiplexing

class ShiftDisplay {

	// MD: for multiplexed drive displays
	// SD: for static drive displays

	private:

		int _latchPin;
		int _clockPin;
		int _dataPin;
		bool _isCathode;
		bool _isMultiplexed;
		bool _isSwapped; // shift registers are swapped (first indexes then segments)
		int _displaySize; // length of whole display
		int _sectionCount; // quantity of display sections
		int _sectionSizes[MAX_DISPLAY_SIZE]; // length of each section
		int _sectionBegins[MAX_DISPLAY_SIZE]; // index where each section begins on whole display
		byte _indexes[MAX_DISPLAY_SIZE]; // each display index in order (encoded)
		byte _cache[MAX_DISPLAY_SIZE]; // value to show on display (encoded in abcdefgp format)

		void construct(int latchPin, int clockPin, int dataPin, DisplayType displayType, const int sectionSizes[], DisplayDrive displayDrive, bool swappedShiftRegisters, const int indexes[]); // common instructions to be called by constructors

		void updateMultiplexedDisplay(); // MD: iterate stored value on each display index, achieving persistence of vision
		void updateStaticDisplay(); // SD: send stored value to whole display
		void clearMultiplexedDisplay(); // MD: clear both shift registers
		void clearStaticDisplay(); // SD: clear all shift registers

		void modifyCache(int index, byte code); // replace a position in cache
		void modifyCache(int beginIndex, int size, const byte codes[]); // replace a interval in cache
		void modifyCacheDot(int index, bool dot); // change dot in a cache position

		void encodeCharacters(int size, const char input[], byte output[], int dotIndex); // encode array of chars to array of bytes in abcdefgp format
		int formatCharacters(int inSize, const char input[], int outSize, char output[], Alignment alignment, bool leadingZeros, int decimalPlaces); // arrange array of chars for displaying in specified alignment, returns dot index on display or -1 if none
		void getCharacters(long input, int size, char output[]); // convert an integer number to an array of chars
		int countCharacters(long number); // calculate the length of an array of chars for an integer number
		int countCharacters(double number); // calculate the length of an array of chars for the integer part on a real number

		void setInteger(long value, bool leadingZeros, Alignment alignment, int section);
		void setReal(double value, int decimalPlaces, bool leadingZeros, Alignment alignment, int section);
		void setNumber(long number, int decimalPlaces, bool leadingZeros, Alignment alignment, int section);
		void setNumber(double number, int decimalPlaces, bool leadingZeros, Alignment alignment, int section);
		void setText(char value, Alignment alignment, int section);
		void setText(const char value[], Alignment alignment, int section);
		void setText(const String &value, Alignment alignment, int section);

		bool isValidSection(int section);

	public:

		// constructors
		ShiftDisplay(DisplayType displayType, int displaySize, DisplayDrive displayDrive = DEFAULT_DRIVE); // default connections, whole display, default pins
		ShiftDisplay(int latchPin, int clockPin, int dataPin, DisplayType displayType, int displaySize, DisplayDrive displayDrive = DEFAULT_DRIVE); // default connections, whole display, custom pins
		ShiftDisplay(DisplayType displayType, const int sectionSizes[], DisplayDrive displayDrive = DEFAULT_DRIVE); // default connections, sectioned display, default pins
		ShiftDisplay(int latchPin, int clockPin, int dataPin, DisplayType displayType, const int sectionSizes[], DisplayDrive displayDrive = DEFAULT_DRIVE); // default connections, sectioned display, custom pins
		ShiftDisplay(DisplayType displayType, int displaySize, bool swappedShiftRegisters, const int indexes[] = DEFAULT_INDEXES); // custom connections, whole display, default pins
		ShiftDisplay(int latchPin, int clockPin, int dataPin, DisplayType displayType, int displaySize, bool swappedShiftRegisters, const int indexes[] = DEFAULT_INDEXES); // custom connections, whole display, custom pins
		ShiftDisplay(DisplayType displayType, const int sectionSizes[], bool swappedShiftRegisters, const int indexes[] = DEFAULT_INDEXES); // custom connections, sectioned display, default pins
		ShiftDisplay(int latchPin, int clockPin, int dataPin, DisplayType displayType, const int sectionSizes[], bool swappedShiftRegisters, const int indexes[] = DEFAULT_INDEXES); // custom connections, sectioned display, custom pins

		// cache value
		void set(int number, int decimalPlaces = DEFAULT_DECIMAL_PLACES_INTEGER, bool leadingZeros = DEFAULT_LEADING_ZEROS, Alignment alignment = DEFAULT_ALIGN_NUMBER);
		void set(int number, bool leadingZeros, Alignment alignment = DEFAULT_ALIGN_NUMBER); // override decimalPlaces obligation
		void set(int number, int decimalPlaces, Alignment alignment); // override leadingZeros obligation
		void set(int number, Alignment alignment); // override decimalPlaces and leadingZeros obligation
		void set(long number, int decimalPlaces = DEFAULT_DECIMAL_PLACES_INTEGER, bool leadingZeros = DEFAULT_LEADING_ZEROS, Alignment alignment = DEFAULT_ALIGN_NUMBER);
		void set(long number, bool leadingZeros, Alignment alignment = DEFAULT_ALIGN_NUMBER); // override decimalPlaces obligation
		void set(long number, int decimalPlaces, Alignment alignment); // override leadingZeros obligation
		void set(long number, Alignment alignment); // override decimalPlaces and leadingZeros obligation
		void set(double number, int decimalPlaces = DEFAULT_DECIMAL_PLACES_REAL, bool leadingZeros = DEFAULT_LEADING_ZEROS, Alignment alignment = DEFAULT_ALIGN_NUMBER);
		void set(double number, bool leadingZeros, Alignment alignment = DEFAULT_ALIGN_NUMBER); // override decimalPlaces obligation
		void set(double number, int decimalPlaces, Alignment alignment); // override leadingZeros obligation
		void set(double number, Alignment alignment); // override decimalPlaces and leadingZeros obligation
		void set(char value, Alignment alignment = DEFAULT_ALIGN_TEXT);
		void set(const char value[], Alignment alignment = DEFAULT_ALIGN_TEXT); // c string
		void set(const String &value, Alignment alignment = DEFAULT_ALIGN_TEXT); // Arduino string object
		void set(const byte customs[]); // custom characters (encoded in abcdefgp format), array length must match display size
		void set(const char characters[], const bool dots[]); // arrays length must match display size
		void setAt(int section, int number, int decimalPlaces = DEFAULT_DECIMAL_PLACES_INTEGER, bool leadingZeros = DEFAULT_LEADING_ZEROS, Alignment alignment = DEFAULT_ALIGN_NUMBER);
		void setAt(int section, int number, bool leadingZeros, Alignment alignment = DEFAULT_ALIGN_NUMBER); // override decimalPlaces obligation
		void setAt(int section, int number, int decimalPlaces, Alignment alignment); // override leadingZeros obligation
		void setAt(int section, int number, Alignment alignment); // override decimalPlaces and leadingZeros obligation
		void setAt(int section, long number, int decimalPlaces = DEFAULT_DECIMAL_PLACES_INTEGER, bool leadingZeros = DEFAULT_LEADING_ZEROS, Alignment alignment = DEFAULT_ALIGN_NUMBER);
		void setAt(int section, long number, bool leadingZeros, Alignment alignment = DEFAULT_ALIGN_NUMBER); // override decimalPlaces obligation
		void setAt(int section, long number, int decimalPlaces, Alignment alignment); // override leadingZeros obligation
		void setAt(int section, long number, Alignment alignment); // override decimalPlaces and leadingZeros obligation
		void setAt(int section, double number, int decimalPlaces = DEFAULT_DECIMAL_PLACES_REAL, bool leadingZeros = DEFAULT_LEADING_ZEROS, Alignment alignment = DEFAULT_ALIGN_NUMBER);
		void setAt(int section, double number, bool leadingZeros, Alignment alignment = DEFAULT_ALIGN_NUMBER); // override decimalPlaces obligation
		void setAt(int section, double number, int decimalPlaces, Alignment alignment); // override leadingZeros obligation
		void setAt(int section, double number, Alignment alignment); // override decimalPlaces and leadingZeros obligation
		void setAt(int section, char value, Alignment alignment = DEFAULT_ALIGN_TEXT);
		void setAt(int section, const char value[], Alignment alignment = DEFAULT_ALIGN_TEXT); // c string
		void setAt(int section, const String &value, Alignment alignment = DEFAULT_ALIGN_TEXT); // Arduino string object
		void setAt(int section, const byte customs[]); // custom characters (encoded in abcdefgp format), array length must match defined section size
		void setAt(int section, const char characters[], const bool dots[]); // arrays length must match defined section size

		// modify cached value at index
		void changeDot(int index, bool dot = DEFAULT_CHANGE_DOT); // show or hide a dot on character
		void changeCharacter(int index, byte custom); // replace with a custom character (encoded in abcdefgp format)
		void changeDotAt(int section, int relativeIndex, bool dot = DEFAULT_CHANGE_DOT); // show or hide a dot on character
		void changeCharacterAt(int section, int relativeIndex, byte custom); // replace with a custom character (encoded in abcdefgp format)

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
		void print(long time, int value, Alignment alignment = DEFAULT_ALIGN_NUMBER); // deprecated by show()
		void print(long time, long value, Alignment alignment = DEFAULT_ALIGN_NUMBER); // deprecated by show()
		void print(long time, double value, int decimalPlaces = DEFAULT_DECIMAL_PLACES_REAL, Alignment alignment = DEFAULT_ALIGN_NUMBER); // deprecated by show()
		void print(long time, double value, Alignment alignment); // deprecated by show()
		void print(long time, char value, Alignment alignment = DEFAULT_ALIGN_TEXT); // deprecated by show()
		void print(long time, const char value[], Alignment alignment = DEFAULT_ALIGN_TEXT); // deprecated by show()
		void print(long time, const String &value, Alignment alignment = DEFAULT_ALIGN_TEXT); // deprecated by show()
		void show(); // deprecated by update()
		void show(int value, unsigned long time, Alignment alignment = DEFAULT_ALIGN_NUMBER); // deprecated by set() show()
		void show(long value, unsigned long time, Alignment alignment = DEFAULT_ALIGN_NUMBER); // deprecated by set() show()
		void show(double valueReal, unsigned long time, int decimalPlaces = DEFAULT_DECIMAL_PLACES_REAL, Alignment alignment = DEFAULT_ALIGN_NUMBER); // deprecated by set() show()
		void show(double valueReal, unsigned long time, Alignment alignment); // deprecated by set() show()
		void show(char value, unsigned long time, Alignment alignment = DEFAULT_ALIGN_TEXT); // deprecated by set() show()
		void show(const char value[], unsigned long time, Alignment alignment = DEFAULT_ALIGN_TEXT); // deprecated by set() show()
		void show(const String &value, unsigned long time, Alignment alignment = DEFAULT_ALIGN_TEXT); // deprecated by set() show()
		void show(const byte customs[], unsigned long time); // deprecated by set() show()
		void show(const char characters[], const bool dots[], unsigned long time); // deprecated by set() show()
		ShiftDisplay(DisplayType displayType, int sectionCount, const int sectionSizes[]);
		ShiftDisplay(int latchPin, int clockPin, int dataPin, DisplayType displayType, int sectionCount, const int sectionSizes[]);
		void setDot(int index, bool dot); // deprecated by changeDot()
		void setDotAt(int section, int relativeIndex, bool dot); // deprecated by changeDotAt()
		void setCustom(int index, byte custom);
		void setCustomAt(int section, int relativeIndex, byte custom);
};

#endif

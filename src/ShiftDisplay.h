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
		int _displayQuantity;
		int _displaySizes[MAX_DISPLAY_SIZE];
		int _displayStarts[MAX_DISPLAY_SIZE]; // index where each display begins
		int _displayTotalSize;
		byte _buffer[MAX_DISPLAY_SIZE]; // value encoded in 7segment format

		void constructSingle(int latchPin, int clockPin, int dataPin, int displayType, int displaySize); // Function with common instructions to be called by single display constructors
		void constructMultiple(int latchPin, int clockPin, int dataPin, int displayType, int displayQuantity, int displaySizes[]); // Function with common instructions to be called by multiple display constructors

		void showDisplay(); // Iterate buffer value on display, achieving persistence of vision
		void clearDisplay(); // Clear shift registers
		void modifyBuffer(int index, byte code); // Change buffer content in a single position
		void modifyBuffer(int startIndex, int size, byte codes[]); // Change buffer content in defined interval
		void modifyBufferDot(int index, bool dot); // Change buffer dot in a single position
		void encodeCharacters(int size, const char input[], byte output[], int dotIndex); // Encode array of chars to array of bytes in 7segment format
		int formatCharacters(int inSize, const char input[], int outSize, char output[], char alignment, int decimalPlaces); // Arrange array of chars for displaying in specified alignment, returns dot index on display or NULL if none
		void getCharacters(long input, int size, char output[]); // Convert an integer number to an array of chars
		int countCharacters(long number); // Calculate the length of an array of chars for an integer number
		int countCharacters(double number); // Calculate the length of an array of chars for the integer part on a real number

	public:

		ShiftDisplay(int displayType, int displaySize); // Initializes the library with the single display information and use default pin numbers
		ShiftDisplay(int latchPin, int clockPin, int dataPin, int displayType, int displaySize); // Initializes the library with the single display information and pin numbers
		ShiftDisplay(int displayType, int displayQuantity, int displaySizes[]); // Initializes the library with the multiple displays information and use default pin numbers
		ShiftDisplay(int latchPin, int clockPin, int dataPin, int displayType, int displayQuantity, int displaySizes[]); // Initializes the library with the multiple displays information and pin numbers

		void set(int value, char alignment = DEFAULT_ALIGN_NUMBER); // Save to buffer an int value
		void set(long value, char alignment = DEFAULT_ALIGN_NUMBER); // Save to buffer a long value
		void set(double value, int decimalPlaces = DEFAULT_DECIMAL_PLACES, char alignment = DEFAULT_ALIGN_NUMBER); // Save to buffer a double value
		void set(double value, char alignment); // needed for overriding decimalPlaces obligation in function above
		void set(char value, char alignment = DEFAULT_ALIGN_TEXT); // Save to buffer a single char value
		void set(const char value[], char alignment = DEFAULT_ALIGN_TEXT); // Save to buffer a char array value
		void set(const String &value, char alignment = DEFAULT_ALIGN_TEXT); // Save to buffer an Arduino String value
		void set(const byte codes[]); // Save to buffer a custom characters array encoded in 7segment format
		void set(const char characters[], bool dots[]); // Save to buffer an array of chars and dots
		void setDot(int index, bool dot); // Modify buffer by inserting or removing a dot at a given index
		void setCustom(int index, byte code); // Modify buffer by replacing at a given index a custom character encoded in 7segment format

		void setAt(int displayId, int value, char alignment = DEFAULT_ALIGN_NUMBER); // Modify buffer at interval... TODO
		void setAt(int displayId, long value, char alignment = DEFAULT_ALIGN_NUMBER); //
		void setAt(int displayId, double value, int decimalPlaces = DEFAULT_DECIMAL_PLACES, char alignment = DEFAULT_ALIGN_NUMBER); //
		void setAt(int displayId, double value, char alignment); // needed for overriding decimalPlaces obligation in function above
		void setAt(int displayId, char value, char alignment = DEFAULT_ALIGN_TEXT); //
		void setAt(int displayId, const char value[], char alignment = DEFAULT_ALIGN_TEXT); //
		void setAt(int displayId, const String &value, char alignment = DEFAULT_ALIGN_TEXT); //
		void setAt(int displayId, const byte codes[]); //
		void setAt(int displayId, const char characters[], bool dots[]); //
		void setDotAt(int displayId, int relativeIndex, bool dot); //
		void setCustomAt(int displayId, int relativeIndex, byte code); //

		void show(); // Show buffer value for one iteration
		void show(unsigned long time); // Show buffer value for the specified time
		void show(int value, unsigned long time, char alignment = DEFAULT_ALIGN_NUMBER); // Save to buffer and show an int value for the specified time
		void show(long value, unsigned long time, char alignment = DEFAULT_ALIGN_NUMBER); // Save to buffer and show a long value for the specified time
		void show(double value, unsigned long time, int decimalPlaces = DEFAULT_DECIMAL_PLACES, char alignment = DEFAULT_ALIGN_NUMBER); // Save to buffer and show a double value for the specified time
		void show(double value, unsigned long time, char alignment); // needed for overriding decimalPlaces obligation in function above
		void show(char value, unsigned long time, char alignment = DEFAULT_ALIGN_TEXT); // Save to buffer and show a char value for the specified time
		void show(const char value[], unsigned long time, char alignment = DEFAULT_ALIGN_TEXT); // Save to buffer and show a char array value for the specified time
		void show(const String &value, unsigned long time, char alignment = DEFAULT_ALIGN_TEXT); // Save to buffer and show an Arduino String value for the specified time
		void show(const byte codes[], unsigned long time); // Save to buffer and show a custom characters array encoded in 7segment format for the specified time
		void show(const char characters[], bool dots[], unsigned long time); // Save to buffer and show an array of chars and dots for the specified time

	// compatibility:
		void insertPoint(int index); // deprecated by setDot()
		void removePoint(int index); // deprecated by setDot()
		void print(long time, int value, char alignment = DEFAULT_ALIGN_NUMBER); // deprecated by show()
		void print(long time, long value, char alignment = DEFAULT_ALIGN_NUMBER); // deprecated by show()
		void print(long time, double value, int decimalPlaces = DEFAULT_DECIMAL_PLACES, char alignment = DEFAULT_ALIGN_NUMBER); // deprecated by show()
		void print(long time, double value, char alignment); // deprecated by show()
		void print(long time, char value, char alignment = DEFAULT_ALIGN_TEXT); // deprecated by show()
		void print(long time, const char value[], char alignment = DEFAULT_ALIGN_TEXT); // deprecated by show()
		void print(long time, const String &value, char alignment = DEFAULT_ALIGN_TEXT); // deprecated by show()
};

#endif

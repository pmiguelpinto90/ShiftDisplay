/*
ShiftDisplay
by Miguel Pynto
Arduino library for driving multiple-digit 7-segment LED displays using 74HC595 shift registers
https://github.com/MiguelPynto/ShiftDisplay
*/


#include "Arduino.h"
#include "ShiftDisplay.h"
#include "CharacterEncoding.h"


// CONSTRUCTORS


// Initializes the library with the display information and use default pin numbers
ShiftDisplay::ShiftDisplay(int displayType, int displaySize) {
	construct(DEFAULT_LATCH_PIN, DEFAULT_CLOCK_PIN, DEFAULT_DATA_PIN, displayType, displaySize);
}


// Initializes the library with the display information and pin numbers
ShiftDisplay::ShiftDisplay(int latchPin, int clockPin, int dataPin, int displayType, int displaySize) {
	construct(latchPin, clockPin, dataPin, displayType, displaySize);
}


// PRIVATE FUNCTIONS


// Function with common instructions to be called by constructors
void ShiftDisplay::construct(int latchPin, int clockPin, int dataPin, int displayType, int displaySize) {
	pinMode(latchPin, OUTPUT);
	pinMode(clockPin, OUTPUT);
	pinMode(dataPin, OUTPUT);
	_latchPin = latchPin;
	_clockPin = clockPin;
	_dataPin = dataPin;
	_displayType = displayType;
	_displaySize = min(displaySize, 8); // displaySize max value is 8
	_povIndexTime = POV_TOTAL_TIME / displaySize; // time spent in each index, achieving pov
	byte initial = displayType ? BLANK : ~BLANK; // initial character for every display index
	memset(_buffer, initial, 8); // fill buffer with initial character
}

// Calculate the length of an array of chars for an integer number
int ShiftDisplay::countCharacters(long number) {
	if (number < 0) // negative number
		return 1 + countCharacters(-number); // minus counts as a character
	if (number < 10)
		return 1;
	return 1 + countCharacters(number / 10);
}


// Calculate the length of an array of chars for the integer part on a real number
int ShiftDisplay::countCharacters(double number) {
	if (number > -1 && number < 0) // -0.x
		return 2; // minus and zero count as 2 characters
	return countCharacters((long) number);
}


// Convert an integer number to an array of characters
void ShiftDisplay::getCharacters(long input, char output[], int size) {

	// invert negative
	bool negative = false;
	if (input < 0) {
		negative = true;
		input = -input;
	}

	// iterate every array position, even if all zeros
	for (int i = size-1; i >= 0; i--) {
		int digit = input % 10;
		char c = digit + '0';
		output[i] = c;
		input /= 10;
	}

	// insert a minus character if negative
	if (negative)
		output[0] = '-';
}


// Arrange characters for display format over specified alignment
// Return point index on display or NULL if none
int ShiftDisplay::formatCharacters(const char input[], int size, char output[], char alignment, int decimalPlaces = NULL) {
	
	// index of character virtual borders
	int left; // lowest index
	int right; // highest index

	int minimum = 0; // minimum display index possible
	int maximum = _displaySize - 1; // maximum display index possible

	// calculate borders according to alignment
	if (alignment == ALIGN_LEFT) {
		left = minimum;
		right = size - 1;
	} else if (alignment == ALIGN_RIGHT) {
		left = _displaySize - size;
		right = maximum;
	} else { // ALIGN_CENTER:
		left = (_displaySize - size) / 2;
		right = left + size - 1;
	}
	
	// fill output array with empty space or characters
	for (int i = 0; i < left; i++) // before characters
		output[i] = ' ';
	for (int i = left, j = 0; i <= right; i++, j++) // characters
		if (i >= minimum && i <= maximum) // not out of bounds on display
			output[i] = input[j];
	for (int i = right+1; i < _displaySize; i++) // after characters
		output[i] = ' ';

	// calcule point index and return it or NULL if none
	if (decimalPlaces == NULL)
		return NULL;
	int pointIndex = right - decimalPlaces;
	if (pointIndex < minimum || pointIndex > maximum) // out of bounds
		return NULL;
	return pointIndex;
}


// Encode array of characters to array of bytes read by the display
void ShiftDisplay::encodeCharacters(const char input[], int pointIndex = NULL) {
	for (int i = 0; i < _displaySize; i++) { // input length = _displaySize
		char c = input[i];
		
		byte code;
		if (c >= '0' && c <= '9')
			code = NUMBERS[c - '0'];
		else if (c >= 'a' && c <= 'z')
			code = LETTERS[c - 'a'];
		else if (c >= 'A' && c <= 'Z')
			code = LETTERS[c - 'A'];
		else if (c == '-')
			code = MINUS;
		else if (c == '!')
			code = EXCLAMATION;
		else if (c == '?')
			code = INTERROGATION;
		else if (c == '.')
			code = POINT;
		else if (c == '_')
			code = UNDERSCORE;
		else if (c == '"' || c == '\'')
			code = QUOTATION;
		else // space or invalid
			code = BLANK;
		
		_buffer[i] = _displayType ? code : ~code;
	}

	// add point to encoded character if needed
	if (pointIndex != NULL)
		_buffer[pointIndex] +=  _displayType ? POINT : ~POINT;
}


// Clear display
void ShiftDisplay::clearDisplay() {
	digitalWrite(_latchPin, LOW);
	shiftOut(_dataPin, _clockPin, MSBFIRST, 0); // both ends of led with same value
	shiftOut(_dataPin, _clockPin, MSBFIRST, 0);
	digitalWrite(_latchPin, HIGH);
}


// Iterate buffer value on display
void ShiftDisplay::showDisplay() {
	for (int i = 0; i < _displaySize; i++) {
		digitalWrite(_latchPin, LOW);

		// data for last shift register
		byte out = _displayType ? ~INDEXES[i] : INDEXES[i];
		shiftOut(_dataPin, _clockPin, LSBFIRST, out);

		// data for first shift register
		shiftOut(_dataPin, _clockPin, LSBFIRST, _buffer[i]);

		digitalWrite(_latchPin, HIGH);

		delay(_povIndexTime); // time showing a single character
	}
}


// PUBLIC FUNCTIONS

/*
// Set up the library with display information
// displayType is COMMON_CATHODE or COMMON_ANODE constant
// displaySize is the quantity of digits of all displays together, maximum of 8
void ShiftDisplay::begin(int displayType, int displaySize) {
	_displayType = displayType;
	_displaySize = min(displaySize, 8);
	_povIndexTime = POV_TOTAL_TIME / _displaySize;
	_buffer[_displaySize];
}
*/

// Save to buffer an int value
void ShiftDisplay::set(int value, char alignment) {
	set((long) value, alignment); // call long function
}


// Save to buffer a long value
void ShiftDisplay::set(long value, char alignment) {
	int size = countCharacters(value);
	char originalCharacters[size];
	getCharacters(value, originalCharacters, size);
	char formattedCharacters[_displaySize];
	formatCharacters(originalCharacters, size, formattedCharacters, alignment);
	encodeCharacters(formattedCharacters);
}


// Save to buffer a double value
void ShiftDisplay::set(double value, int decimalPlaces, char alignment) {

	// if no decimal places, call integer function instead
	if (decimalPlaces == 0) {
		long newValue = round(value);
		set(newValue, alignment);
		return;
	}

 	// calculate value with specified decimal places as integer (eg 1.236, 2 = 124)
	long newValue = round(value * pow(10, decimalPlaces));

	int size = countCharacters(value) + decimalPlaces;
	char originalCharacters[size];
	getCharacters(newValue, originalCharacters, size);
	char formattedCharacters[_displaySize];
	int pointIndex = formatCharacters(originalCharacters, size, formattedCharacters, alignment, decimalPlaces);
	encodeCharacters(formattedCharacters, pointIndex);
}


// Save to buffer a double value with default number of decimal places
void ShiftDisplay::set(double value, char alignment) {
	set(value, DEFAULT_DECIMAL_PLACES, alignment);
}


// Save to buffer a char value
void ShiftDisplay::set(char value, char alignment) {
	char originalCharacters[] = {value};
	char formattedCharacters[_displaySize];
	formatCharacters(originalCharacters, 1, formattedCharacters, alignment);
	encodeCharacters(formattedCharacters);
}


// Save to buffer a char array value
void ShiftDisplay::set(const char value[], char alignment) {
	int size = strlen(value);
	char formattedCharacters[_displaySize];
	formatCharacters(value, size, formattedCharacters, alignment);
	encodeCharacters(formattedCharacters);
}


// Save to buffer a string object value
void ShiftDisplay::set(const String &value, char alignment) {
	set(value.c_str(), alignment); // call char array function
}


// Show buffer value for one iteration
void ShiftDisplay::show() {
	showDisplay();
	clearDisplay();
}


// Show buffer value for the specified time
void ShiftDisplay::show(long time) {
	long end = millis() + time - POV_TOTAL_TIME; // start + total duration - last iteration (so it doesnt exceed time requested)
	while ((long)millis() <= end)
		showDisplay();
	clearDisplay();
}


// Save to buffer and show an int value for the specified time
void ShiftDisplay::print(long time, int value, char alignment) {
	set(value, alignment);
	show(time);
}


// Save to buffer and show a long value for the specified time
void ShiftDisplay::print(long time, long value, char alignment) {
	set(value, alignment);
	show(time);
}


// Save to buffer and show a double value for the specified time
void ShiftDisplay::print(long time, double value, int decimalPlaces, char alignment) {
	set(value, decimalPlaces, alignment);
	show(time);
}


// Save to buffer and show a double value with default number of decimal places for the specified time
void ShiftDisplay::print(long time, double value, char alignment) {
	set(value, DEFAULT_DECIMAL_PLACES, alignment);
	show(time);
}


// Save to buffer and show a char value for the specified time
void ShiftDisplay::print(long time, char value, char alignment) {
	set(value, alignment);
	show(time);
}


// Save to buffer and show a char array value for the specified time
void ShiftDisplay::print(long time, const char value[], char alignment) {
	set(value, alignment);
	show(time);
}


// Save to buffer and show a string object value for the specified time
void ShiftDisplay::print(long time, const String &value, char alignment) {
	set(value, alignment);
	show(time);
}

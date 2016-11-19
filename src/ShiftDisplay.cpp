// github.com/MiguelPynto/ShiftDisplay


#include "Arduino.h"
#include "ShiftDisplay.h"
#include "ShiftDisplayConstants.h"


// CONSTRUCTORS


// Initializes the library with the pin numbers and display information
// latchPin, clockPin and dataPin are the shift register pins connected to the Arduino digital outputs
// displayType is COMMON_CATHODE or COMMON_ANODE constant
// displaySize is the quantity of digits of all displays together, maximum of 8
ShiftDisplay::ShiftDisplay(int latchPin, int clockPin, int dataPin, int displayType, int displaySize) {
	pinMode(latchPin, OUTPUT);
	pinMode(clockPin, OUTPUT);
	pinMode(dataPin, OUTPUT);
	_latchPin = latchPin;
	_clockPin = clockPin;
	_dataPin = dataPin;
	_displayType = displayType;
	_displaySize = min(displaySize, 8);
	_povDelay = POV / _displaySize;
}


// PRIVATE FUNCTIONS


// Count the characters in an integer number
// If the number is negative, the minus also counts as a character
int ShiftDisplay::countCharacters(long number) {
	if (number < 0)
		return 1 + countCharacters(-number);
	if (number < 10)
		return 1;
	return 1 + countCharacters(number / 10);
}


// Count the integer characters in a real number
// If the number is negative, the minus also counts as a character
// If the number is negative zero (eg -0.42), the minus and zero count as two characters
int ShiftDisplay::countCharacters(double number) {
	if (number > -1 && number < 0)
		return 2;
	return countCharacters((long) number);
}


// Count the number of characters in a char array
int ShiftDisplay::countCharacters(const char text[]) {
	return strlen(text);
}


// Convert a number to an array of characters
void ShiftDisplay::getCharacters(long input, char output[], int size) {
	
	// if negative, insert a minus character
	if (input < 0) {
		input = -input;
		output[0] = '-';
	}

	int i = size - 1;
	do { // still enter if zero
		int digit = input % 10;
		char c = digit + '0';
		output[i--] = c;
		input /= 10;
	} while (input != 0);
}


// Format characters to display length over specified alignment
void ShiftDisplay::formatCharacters(const char input[], int size, char output[], int alignment) {
	formatCharacters(input, size, output, alignment, -1);
}


// Format characters to display length over specified alignment, and calculate point position based on decimalPlaces
// Return point position on display or -1 if there is no point
int ShiftDisplay::formatCharacters(const char input[], int size, char output[], int alignment, int decimalPlaces) {
	
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

	// calcule point position and return it
	int pointIndex = -1;
	if (decimalPlaces != -1) {
		pointIndex = right - decimalPlaces;
		if (pointIndex < minimum || pointIndex > maximum) // out of bounds
			pointIndex = -1;
	}
	return pointIndex;
}


// Encode array of characters to array of bytes read by the display
void ShiftDisplay::encodeCharacters(const char input[]) {
	encodeCharacters(input, -1);
}


// Encode array of characters and point to array of bytes read by the display
void ShiftDisplay::encodeCharacters(const char input[], int pointIndex) {
	for (int i = 0; i < _displaySize; i++) {
		char c = input[i];
		
		byte code;
		if (c >= 'A' && c <= 'Z') {
			code = LETTERS[c - 'A'];
		} else if (c >= 'a' && c <= 'z') {
			code = LETTERS[c - 'a'];
		} else if (c >= '0' && c <= '9') {
			code = NUMBERS[c - '0'];
		} else if (c == '-') {
			code = MINUS;
		} else { // space or invalid
			code = BLANK;
		}

		_buffer[i] = _displayType ? code : ~code;
	}

	// add point to encoded character if needed
	if (pointIndex != -1)
		_buffer[pointIndex] +=  _displayType ? POINT : ~POINT;
}


// Clear display
void ShiftDisplay::clearDisplay() {
	digitalWrite(_latchPin, LOW);
	shiftOut(_dataPin, _clockPin, MSBFIRST, 0); // both ends of led with same value
	shiftOut(_dataPin, _clockPin, MSBFIRST, 0);
	digitalWrite(_latchPin, HIGH);
}


// Show buffer value on display
void ShiftDisplay::showDisplay() {
	for (int i = 0; i < _displaySize; i++) {
		digitalWrite(_latchPin, LOW);

		// data for last shift register
		byte out = _displayType ? ~DIGITS[i] : DIGITS[i];
		shiftOut(_dataPin, _clockPin, LSBFIRST, out);

		// data for first shift register
		shiftOut(_dataPin, _clockPin, LSBFIRST, _buffer[i]);

		digitalWrite(_latchPin, HIGH);

		delay(_povDelay); // time showing a single character
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
	_povDelay = POV / _displaySize;
	_buffer[_displaySize];
}
*/

// Save to buffer an int value, right aligned by default
void ShiftDisplay::set(int value, int alignment) {
	set((long) value, alignment); // call long function
}


// Save to buffer a long value, right aligned by default
void ShiftDisplay::set(long value, int alignment) {
	int size = countCharacters(value);
	char originalCharacters[size];
	getCharacters(value, originalCharacters, size);
	char formattedCharacters[_displaySize];
	formatCharacters(originalCharacters, size, formattedCharacters, alignment);
	encodeCharacters(formattedCharacters);
	// return max(_displaySize - size, 0);
}


// Save to buffer a double value, with 2 decimal places and right aligned by default
void ShiftDisplay::set(double value, int decimalPlaces, int alignment) {

	// if no decimal places, call integer function instead
	if (decimalPlaces == 0) {
		long newValue = round(value);
		set(newValue, alignment);
		return;
	}

 	// calculate value with specified decimal places as integer (eg 1.236, 2 = 124)
	long newValue = round(value * pow(10, decimalPlaces));

	int size = countCharacters(newValue);
	char originalCharacters[size];
	getCharacters(newValue, originalCharacters, size);
	char formattedCharacters[_displaySize];
	int pointIndex = formatCharacters(originalCharacters, size, formattedCharacters, alignment, decimalPlaces);
	encodeCharacters(formattedCharacters, pointIndex);
}


// Save to buffer a char value, center aligned by default
// Valid characters are A-Z, a-z, 0-9, -, space
void ShiftDisplay::set(char value, int alignment) {
	char originalCharacters[] = {value};
	char formattedCharacters[_displaySize];
	formatCharacters(originalCharacters, 1, formattedCharacters, alignment);
	encodeCharacters(formattedCharacters);
}


// Save to buffer a char array value, left aligned by default
// Valid characters are A-Z, a-z, 0-9, -, space
void ShiftDisplay::set(const char value[], int alignment) {
	int size = countCharacters(value);
	char formattedCharacters[_displaySize];
	formatCharacters(value, size, formattedCharacters, alignment);
	encodeCharacters(formattedCharacters);
}


// Save to buffer a string object value, left aligned by default
// Valid characters are A-Z, a-z, 0-9, -, space
void ShiftDisplay::set(const String &value, int alignment) {
	set(value.c_str(), alignment); // call char array function
}


// Show buffer value for one iteration
void ShiftDisplay::show() {
	showDisplay();
	clearDisplay();
}


// Show buffer value for the specified time in milliseconds
void ShiftDisplay::show(long time) {
	long end = millis() + time - POV; // start + total duration - last iteration (so it doesnt exceed time requested)
	while ((long)millis() <= end)
		showDisplay();
	clearDisplay();
}


// Save to buffer and show an int value for the specified time in milliseconds, right aligned by default
void ShiftDisplay::print(long time, int value, int alignment) {
	set(value, alignment);
	show(time);
}


// Save to buffer and show a long value for the specified time in milliseconds, right aligned by default
void ShiftDisplay::print(long time, long value, int alignment) {
	set(value, alignment);
	show(time);
}


// Save to buffer and show a double value for the specified time in milliseconds, with 2 decimal places and right aligned by default
void ShiftDisplay::print(long time, double value, int decimalPlaces, int alignment) {
	set(value, decimalPlaces, alignment);
	show(time);
}


// Save to buffer and show a char value for the specified time in milliseconds, center aligned by default
// Valid characters are A-Z, a-z, 0-9, -, space
void ShiftDisplay::print(long time, char value, int alignment) {
	set(value, alignment);
	show(time);
}


// Save to buffer and show a char array value for the specified time in milliseconds, center aligned by default
// Valid characters are A-Z, a-z, 0-9, -, space
void ShiftDisplay::print(long time, const char value[], int alignment) {
	set(value, alignment);
	show(time);
}


// Save to buffer and show a string object value for the specified time in milliseconds, left aligned by default
// Valid characters are A-Z, a-z, 0-9, -, space
void ShiftDisplay::print(long time, const String &value, int alignment) {
	set(value, alignment);
	show(time);
}

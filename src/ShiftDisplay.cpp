/*
ShiftDisplay
by MiguelPynto
Arduino library for driving multiple-digit 7-segment LED displays using 74HC595 shift registers
https://miguelpynto.github.io/ShiftDisplay/
*/

#include "Arduino.h"
#include "ShiftDisplay.h"
#include "CharacterEncoding.h"

// *****************************************************************************
// CONSTRUCTORS

// Initializes the library with the single display information and use default pin numbers
ShiftDisplay::ShiftDisplay(int displayType, int displaySize) {
	constructSingle(DEFAULT_LATCH_PIN, DEFAULT_CLOCK_PIN, DEFAULT_DATA_PIN, displayType, displaySize);
}

// Initializes the library with the single display information and pin numbers
ShiftDisplay::ShiftDisplay(int latchPin, int clockPin, int dataPin, int displayType, int displaySize) {
	constructSingle(latchPin, clockPin, dataPin, displayType, displaySize);
}

// Initializes the library with the multiple displays information and use default pin numbers
ShiftDisplay::ShiftDisplay(int displayType, int displayQuantity, int displaySizes[]) {
	constructMultiple(DEFAULT_LATCH_PIN, DEFAULT_CLOCK_PIN, DEFAULT_DATA_PIN, displayType, displayQuantity, displaySizes);
}

// Initializes the library with the multiple displays information and pin numbers
ShiftDisplay::ShiftDisplay(int latchPin, int clockPin, int dataPin, int displayType, int displayQuantity, int displaySizes[]) {
	constructMultiple(latchPin, clockPin, dataPin, displayType, displayQuantity, displaySizes);
}

// *****************************************************************************
// PRIVATE FUNCTIONS

// Function with common instructions to be called by single display constructors
void ShiftDisplay::constructSingle(int latchPin, int clockPin, int dataPin, int displayType, int displaySize) {
	pinMode(latchPin, OUTPUT);
	pinMode(clockPin, OUTPUT);
	pinMode(dataPin, OUTPUT);
	_latchPin = latchPin;
	_clockPin = clockPin;
	_dataPin = dataPin;

	displaySize = min(displaySize, MAX_DISPLAY_SIZE); // override if displaySize is too big
	_displayType = displayType;
	_displayQuantity = 1;
	_displaySizes[0] = displaySize;
	_displayStarts[0] = 0;
	_displaySize = displaySize;

	byte initial = displayType ? BLANK : ~BLANK; // initial character for every display index
	memset(_buffer, initial, MAX_DISPLAY_SIZE); // fill buffer with initial character
}

// Function with common instructions to be called by multiple display constructors
void ShiftDisplay::constructMultiple(int latchPin, int clockPin, int dataPin, int displayType, int displayQuantity, int displaySizes[]) {
	pinMode(latchPin, OUTPUT);
	pinMode(clockPin, OUTPUT);
	pinMode(dataPin, OUTPUT);
	_latchPin = latchPin;
	_clockPin = clockPin;
	_dataPin = dataPin;

	int i = 0;
	int displaySize = 0;
	for (; i < displayQuantity && displaySize < MAX_DISPLAY_SIZE && i < MAX_DISPLAY_SIZE; i++) {
		int preSize = displaySize + displaySizes[i]; // preview new size
		if (preSize <= MAX_DISPLAY_SIZE) { // size is ok
			_displaySizes[i] = displaySizes[i];
			_displayStarts[i] = displaySize;
			displaySize = preSize;
		} else { // size is out of bounds
			_displaySizes[i] = MAX_DISPLAY_SIZE - displaySize; // override last size to until max
			_displayStarts[i] = displaySize;
			displaySize = MAX_DISPLAY_SIZE; // override size to max
		}
	}
	_displayQuantity = i;
	_displaySize = displaySize;
	_displayType = displayType;

	byte initial = displayType ? BLANK : ~BLANK; // initial character for every display index
	memset(_buffer, initial, MAX_DISPLAY_SIZE); // fill buffer with initial character
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
void ShiftDisplay::getCharacters(long input, int size, char output[]) {

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
// Return dot index on display or NULL if none
int ShiftDisplay::formatCharacters(int inSize, const char input[], int outSize, char output[], char alignment, int decimalPlaces = NULL) {
	
	// index of character virtual borders
	int left; // lowest index
	int right; // highest index

	int minimum = 0; // minimum display index possible
	int maximum = outSize - 1; // maximum display index possible

	// calculate borders according to alignment
	if (alignment == ALIGN_LEFT) {
		left = minimum;
		right = inSize - 1;
	} else if (alignment == ALIGN_RIGHT) {
		left = outSize - inSize;
		right = maximum;
	} else { // ALIGN_CENTER:
		left = (outSize - inSize) / 2;
		right = left + inSize - 1;
	}
	
	// fill output array with empty space or characters
	for (int i = 0; i < left; i++) // before characters
		output[i] = ' ';
	for (int i = left, j = 0; i <= right; i++, j++) // characters
		if (i >= minimum && i <= maximum) // not out of bounds on display
			output[i] = input[j];
	for (int i = right+1; i < outSize; i++) // after characters
		output[i] = ' ';

	// calculate dot index and return it or NULL if none
	if (decimalPlaces == NULL)
		return NULL;
	int dotIndex = right - decimalPlaces;
	if (dotIndex < minimum || dotIndex > maximum) // out of bounds
		return NULL;
	return dotIndex;
}

// Encode array of characters to array of bytes read by the display
void ShiftDisplay::encodeCharacters(int size, const char input[], byte output[], int dotIndex = NULL) {
	for (int i = 0; i < size; i++) {
		char c = input[i];
		
		if (c >= '0' && c <= '9')
			output[i] = NUMBERS[c - '0'];
		else if (c >= 'a' && c <= 'z')
			output[i] = LETTERS[c - 'a'];
		else if (c >= 'A' && c <= 'Z')
			output[i] = LETTERS[c - 'A'];
		else if (c == '-')
			output[i] = MINUS;
		else // space or invalid
			output[i] = BLANK;
	}

	if (dotIndex != NULL)
		bitWrite(output[dotIndex], 0, 1);
}

// Modify buffer, add or remove a dot at index
void ShiftDisplay::setBufferDot(int index, bool show) {
	int bit;
	if (show)
		bit = _displayType ? 1 : 0;
	else
		bit = _displayType ? 0 : 1;
	//int bit = (show == (bool)_displayType) TODO
	bitWrite(_buffer[index], 0, bit);
}

// Save data to buffer, ready to be read by display
void ShiftDisplay::setBuffer(int start, int size, byte input[]) {
	for (int i = 0; i < size; i++)
		_buffer[i+start] = _displayType ? input[i] : ~input[i];
}

// Clear display
void ShiftDisplay::clearDisplay() {
	digitalWrite(_latchPin, LOW);
	shiftOut(_dataPin, _clockPin, MSBFIRST, 0); // both ends of led with same value
	shiftOut(_dataPin, _clockPin, MSBFIRST, 0);
	digitalWrite(_latchPin, HIGH);
}

// Iterate buffer value on display, achieving persistence of vision
void ShiftDisplay::printDisplay() {
	for (int i = 0; i < _displaySize; i++) {
		digitalWrite(_latchPin, LOW);

		// data for last shift register
		byte out = _displayType ? ~INDEXES[i] : INDEXES[i];
		shiftOut(_dataPin, _clockPin, LSBFIRST, out);

		// data for first shift register
		shiftOut(_dataPin, _clockPin, LSBFIRST, _buffer[i]);

		digitalWrite(_latchPin, HIGH);

		delay(1); // milliseconds showing a single character
	}
}

// *****************************************************************************
// PUBLIC FUNCTIONS

// Save to buffer an int value
void ShiftDisplay::set(int value, char alignment) {
	set((long) value, alignment); // call long function
}

// Save to buffer a long value
void ShiftDisplay::set(long value, char alignment) {
	int size = countCharacters(value);
	char originalCharacters[size];
	getCharacters(value, size, originalCharacters);
	char formattedCharacters[_displaySize];
	formatCharacters(size, originalCharacters, _displaySize, formattedCharacters, alignment);
	byte encodedCharacters[_displaySize];
	encodeCharacters(_displaySize, formattedCharacters, encodedCharacters);
	setBuffer(0, _displaySize, encodedCharacters);
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
	getCharacters(newValue, size, originalCharacters);
	char formattedCharacters[_displaySize];
	int dotIndex = formatCharacters(size, originalCharacters, _displaySize, formattedCharacters, alignment, decimalPlaces);
	byte encodedCharacters[_displaySize];
	encodeCharacters(_displaySize, formattedCharacters, encodedCharacters, dotIndex);
	setBuffer(0, _displaySize, encodedCharacters);
}

// Save to buffer a double value with default number of decimal places
void ShiftDisplay::set(double value, char alignment) {
	set(value, DEFAULT_DECIMAL_PLACES, alignment);
}

// Save to buffer a char value
void ShiftDisplay::set(char value, char alignment) {
	char originalCharacters[] = {value};
	char formattedCharacters[_displaySize];
	formatCharacters(1, originalCharacters, _displaySize, formattedCharacters, alignment);
	byte encodedCharacters[_displaySize];
	encodeCharacters(_displaySize, formattedCharacters, encodedCharacters);
	setBuffer(0, _displaySize, encodedCharacters);
}

// Save to buffer a char array value
void ShiftDisplay::set(const char value[], char alignment) {
	int size = strlen(value);
	char formattedCharacters[_displaySize];
	formatCharacters(size, value, _displaySize, formattedCharacters, alignment);
	byte encodedCharacters[_displaySize];
	encodeCharacters(_displaySize, formattedCharacters, encodedCharacters);
	setBuffer(0, _displaySize, encodedCharacters);
}

// Save to buffer an Arduino String value, manual processing for better support between Arduino cores
void ShiftDisplay::set(const String &value, char alignment) {

	// get String size
	int size = 0;
	while (value[size] != '\0')
		size++;
	
	// convert String to char array
	char str[size + 1];
	for (int i = 0; i < size; i++)
		str[i] = value[i];
	str[size] = '\0';

	set(str, alignment); // call char array function
}

// Save to buffer a formatted segments array
void ShiftDisplay::set(const byte codes[]) {
	setBuffer(0, _displaySize, codes);
}

// Save to buffer a formatted characters array and a dots array
void ShiftDisplay::set(const char characters[], bool dots[]) {
	byte encodedCharacters[_displaySize];
	encodeCharacters(_displaySize, characters, encodedCharacters);
	setBuffer(0, _displaySize, encodedCharacters);
	if (dots != NULL)
		for (int i = 0; i < _displaySize; i++)
			setBufferDot(i, dots[i]);
}

// Save to a section of buffer an int value
void ShiftDisplay::setAt(int displayId, int value, char alignment) {
	setAt(displayId, (long) value, alignment); // call long function
}

// Save to a section of buffer a long value
void ShiftDisplay::setAt(int displayId, long value, char alignment) {
	if (displayId >= 0 && displayId < _displayQuantity) { // valid displayId
		int valueSize = countCharacters(value);
		char originalCharacters[valueSize];
		getCharacters(value, valueSize, originalCharacters);
		int displaySize = _displaySizes[displayId];
		char formattedCharacters[displaySize];
		formatCharacters(valueSize, originalCharacters, displaySize, formattedCharacters, alignment);
		byte encodedCharacters[displaySize];
		encodeCharacters(displaySize, formattedCharacters, encodedCharacters);
		setBuffer(_displayStarts[displayId], displaySize, encodedCharacters);
	}
}

// Modify buffer, insert dot at index
void ShiftDisplay::insertDot(int index) {
	if (index >= 0 && index < _displaySize)
		setBufferDot(index, true);
}

// Modify buffer, remove dot at index
void ShiftDisplay::removeDot(int index) {
	if (index >= 0 && index < _displaySize)
		setBufferDot(index, false);
}

// Show buffer value for one iteration
void ShiftDisplay::show() {
	printDisplay();
	clearDisplay();
}

// Show buffer value for the specified time
void ShiftDisplay::show(unsigned long time) {
	unsigned long end = millis() + time - (1*_displaySize); // start + total duration - last iteration (so it doesnt exceed time requested)
	while (millis() <= end)
		printDisplay();
	clearDisplay();
}

// Save to buffer and show an int value for the specified time
void ShiftDisplay::show(int value, unsigned long time, char alignment) {
	set(value, alignment);
	show(time);
}

// Save to buffer and show a long value for the specified time
void ShiftDisplay::show(long value, unsigned long time, char alignment) {
	set(value, alignment);
	show(time);
}

// Save to buffer and show a double value for the specified time
void ShiftDisplay::show(double value, unsigned long time, int decimalPlaces, char alignment) {
	set(value, decimalPlaces, alignment);
	show(time);
}

// Save to buffer and show a double value with default number of decimal places for the specified time
void ShiftDisplay::show(double value, unsigned long time, char alignment) {
	set(value, DEFAULT_DECIMAL_PLACES, alignment);
	show(time);
}

// Save to buffer and show a char value for the specified time
void ShiftDisplay::show(char value, unsigned long time, char alignment) {
	set(value, alignment);
	show(time);
}

// Save to buffer and show a char array value for the specified time
void ShiftDisplay::show(const char value[], unsigned long time, char alignment) {
	set(value, alignment);
	show(time);
}

// Save to buffer and show an Arduino String value for the specified time
void ShiftDisplay::show(const String &value, unsigned long time, char alignment) {
	set(value, alignment);
	show(time);
}

// Save to buffer and show a formatted segments array for the specified time
void ShiftDisplay::show(const byte codes[], unsigned long time) {
	set(codes);
	show(time);
}

// Save to buffer and show a formatted characters array and a dots array for the specified time
void ShiftDisplay::show(const char characters[], bool dots[], unsigned long time) {
	set(characters, dots);
	show(time);
}

// Duplicates to retain compatibility with old versions
void ShiftDisplay::insertPoint(int index) { insertDot(index); }
void ShiftDisplay::removePoint(int index) { removeDot(index); }
void ShiftDisplay::print(long time, int value, char alignment) { show(value, time, alignment); }
void ShiftDisplay::print(long time, long value, char alignment) { show(value, time, alignment); }
void ShiftDisplay::print(long time, double value, int decimalPlaces, char alignment) { show(value, time, decimalPlaces, alignment); }
void ShiftDisplay::print(long time, double value, char alignment) { show(value, time, alignment); }
void ShiftDisplay::print(long time, char value, char alignment) { show(value, time, alignment); }
void ShiftDisplay::print(long time, const char value[], char alignment) { show(value, time, alignment); }
void ShiftDisplay::print(long time, const String &value, char alignment) { show(value, time, alignment); }

/*
ShiftDisplay
by MiguelPynto
Arduino library for driving multiple-digit 7-segment LED displays using 74HC595 shift registers
https://miguelpynto.github.io/ShiftDisplay/
*/

#include "Arduino.h"
#include "ShiftDisplay.h"
#include "CharacterEncoding.h"

// CONSTRUCTORS ****************************************************************

ShiftDisplay::ShiftDisplay(DisplayType displayType, int displaySize) {
	int sectionSizes[] = {displaySize};
	construct(DEFAULT_LATCH_PIN, DEFAULT_CLOCK_PIN, DEFAULT_DATA_PIN, displayType, 1, sectionSizes);
}

ShiftDisplay::ShiftDisplay(int latchPin, int clockPin, int dataPin, DisplayType displayType, int displaySize) {
	int sectionSizes[] = {displaySize};
	construct(latchPin, clockPin, dataPin, displayType, 1, sectionSizes);
}

ShiftDisplay::ShiftDisplay(DisplayType displayType, int sectionCount, int sectionSizes[]) {
	construct(DEFAULT_LATCH_PIN, DEFAULT_CLOCK_PIN, DEFAULT_DATA_PIN, displayType, sectionCount, sectionSizes);
}

ShiftDisplay::ShiftDisplay(int latchPin, int clockPin, int dataPin, DisplayType displayType, int sectionCount, int sectionSizes[]) {
	construct(latchPin, clockPin, dataPin, displayType, sectionCount, sectionSizes);
}

void ShiftDisplay::initPins(int latchPin, int clockPin, int dataPin) {
	_latchPin = latchPin;
	_clockPin = clockPin;
	_dataPin = dataPin;
	pinMode(_latchPin, OUTPUT);
	pinMode(_clockPin, OUTPUT);
	pinMode(_dataPin, OUTPUT);
}

void ShiftDisplay::construct(int latchPin, int clockPin, int dataPin, DisplayType displayType, int sectionCount, int sectionSizes[]) {
	initPins(latchPin, clockPin, dataPin);

	_isCathode = displayType == COMMON_CATHODE || displayType == INDIVIDUAL_CATHODE;
	_isMultiplex = displayType == COMMON_CATHODE || displayType == COMMON_ANODE;
	int i = 0;
	int displaySize = 0;
	for (; i < sectionCount && displaySize < MAX_DISPLAY_SIZE && i < MAX_DISPLAY_SIZE; i++) {
		int preTotalSize = displaySize + sectionSizes[i]; // preview new size
		if (preTotalSize <= MAX_DISPLAY_SIZE) { // size is ok
			_sectionSizes[i] = sectionSizes[i];
			_sectionBegins[i] = displaySize;
			displaySize = preTotalSize;
		} else { // size is out of bounds
			_sectionSizes[i] = MAX_DISPLAY_SIZE - displaySize; // override last size to until max
			_sectionBegins[i] = displaySize;
			displaySize = MAX_DISPLAY_SIZE; // override size to max
		}
	}
	_sectionCount = i;
	_displaySize = displaySize;

	byte empty = _isCathode ? EMPTY : ~EMPTY;
	memset(_storage, empty, MAX_DISPLAY_SIZE); // fill storage with empty character
	if (_isMultiplex)
		clearMultiplexDisplay();
	else
		clearConstantDisplay();
}

// PRIVATE FUNCTIONS ***********************************************************

void ShiftDisplay::showMultiplexDisplay() {
	for (int i = 0; i < _displaySize; i++) {
		digitalWrite(_latchPin, LOW);

		// data for last shift register
		byte out = _isCathode ? ~INDEXES[i] : INDEXES[i];
		shiftOut(_dataPin, _clockPin, LSBFIRST, out);

		// data for first shift register
		shiftOut(_dataPin, _clockPin, LSBFIRST, _storage[i]);

		digitalWrite(_latchPin, HIGH);

		delay(POV);
	}
}

void ShiftDisplay::setConstantDisplay() {
	digitalWrite(_latchPin, LOW);
	for (int i = _displaySize - 1; i >= 0 ; i--)
		shiftOut(_dataPin, _clockPin, LSBFIRST, _storage[i]);
	digitalWrite(_latchPin, HIGH);
}

void ShiftDisplay::clearMultiplexDisplay() {
	digitalWrite(_latchPin, LOW);
	shiftOut(_dataPin, _clockPin, MSBFIRST, EMPTY); // 0 at both ends of led
	shiftOut(_dataPin, _clockPin, MSBFIRST, EMPTY);
	digitalWrite(_latchPin, HIGH);
}

void ShiftDisplay::clearConstantDisplay() {
	digitalWrite(_latchPin, LOW);
	byte empty = _isCathode ? EMPTY : ~EMPTY;
	for (int i = 0; i < _displaySize; i++)
		shiftOut(_dataPin, _clockPin, MSBFIRST, empty);
	digitalWrite(_latchPin, HIGH);
}

void ShiftDisplay::modifyStorage(int index, byte code) {
	_storage[index] = _isCathode ? code : ~code;
}

void ShiftDisplay::modifyStorage(int beginIndex, int size, byte codes[]) {
	for (int i = 0; i < size; i++)
		_storage[i+beginIndex] = _isCathode ? codes[i] : ~codes[i];
}

void ShiftDisplay::modifyStorageDot(int index, bool dot) {
	bool bit = _isCathode ? dot : !dot;
	bitWrite(_storage[index], 0, bit);
}

void ShiftDisplay::encodeCharacters(int size, const char input[], byte output[], int dotIndex = -1) {
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
			output[i] = EMPTY;
	}
	
	if (dotIndex != -1)
		bitWrite(output[dotIndex], 0, 1);
}

int ShiftDisplay::formatCharacters(int inSize, const char input[], int outSize, char output[], char alignment, int decimalPlaces = -1) {
	
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

	// calculate dot index and return it or -1 if none
	if (decimalPlaces == -1)
		return -1;
	int dotIndex = right - decimalPlaces;
	if (dotIndex < minimum || dotIndex > maximum) // out of bounds
		return -1;
	return dotIndex;
}

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

int ShiftDisplay::countCharacters(long number) {
	if (number < 0) // negative number
		return 1 + countCharacters(-number); // minus counts as a character
	if (number < 10)
		return 1;
	return 1 + countCharacters(number / 10);
}

int ShiftDisplay::countCharacters(double number) {
	if (number > -1 && number < 0) // -0.x
		return 2; // minus and zero count as 2 characters
	return countCharacters((long) number);
}

// PUBLIC FUNCTIONS ************************************************************

void ShiftDisplay::set(int value, char alignment) {
	setAt(0, value, alignment);
}

void ShiftDisplay::set(long value, char alignment) {
	setAt(0, value, alignment);
}

void ShiftDisplay::set(double valueReal, int decimalPlaces, char alignment) {
	setAt(0, valueReal, decimalPlaces, alignment);
}

void ShiftDisplay::set(double valueReal, char alignment) {
	setAt(0, valueReal, alignment);
}

void ShiftDisplay::set(char value, char alignment) {
	setAt(0, value, alignment);
}

void ShiftDisplay::set(const char value[], char alignment) {
	setAt(0, value, alignment);
}

void ShiftDisplay::set(const String &value, char alignment) {
	setAt(0, value, alignment);
}

void ShiftDisplay::set(const byte customs[]) {
	setAt(0, customs);
}

void ShiftDisplay::set(const char characters[], bool dots[]) {
	setAt(0, characters, dots);
}

void ShiftDisplay::setDot(int index, bool dot) {
	setDotAt(0, index, dot);
}

void ShiftDisplay::setCustom(int index, byte custom) {
	setCustomAt(0, index, custom);
}

void ShiftDisplay::setAt(int section, int value, char alignment) {
	setAt(section, (long) value, alignment); // call long function
}

void ShiftDisplay::setAt(int section, long value, char alignment) {
	if (section >= 0 && section < _sectionCount) { // valid section
		int valueSize = countCharacters(value);
		char originalCharacters[valueSize];
		getCharacters(value, valueSize, originalCharacters);
		int sectionSize = _sectionSizes[section];
		char formattedCharacters[sectionSize];
		formatCharacters(valueSize, originalCharacters, sectionSize, formattedCharacters, alignment);
		byte encodedCharacters[sectionSize];
		encodeCharacters(sectionSize, formattedCharacters, encodedCharacters);
		modifyStorage(_sectionBegins[section], sectionSize, encodedCharacters);
		if (!_isMultiplex)
			setConstantDisplay();
	}
}

void ShiftDisplay::setAt(int section, double valueReal, int decimalPlaces, char alignment) {
	if (section >= 0 && section < _sectionCount) { // valid section
		
		// if no decimal places, call long function instead
		if (decimalPlaces == 0) {
			long value = round(valueReal);
			setAt(section, value, alignment);
			return;
		}
	
		 // calculate value with specified decimal places as integer (eg 1.236, 2 = 124)
		long value = round(valueReal * pow(10, decimalPlaces));
	
		int valueSize = countCharacters(valueReal) + decimalPlaces;
		char originalCharacters[valueSize];
		getCharacters(value, valueSize, originalCharacters);
		int sectionSize = _sectionSizes[section];
		char formattedCharacters[sectionSize];
		int dotIndex = formatCharacters(valueSize, originalCharacters, sectionSize, formattedCharacters, alignment, decimalPlaces);
		byte encodedCharacters[sectionSize];
		encodeCharacters(sectionSize, formattedCharacters, encodedCharacters, dotIndex);
		modifyStorage(_sectionBegins[section], sectionSize, encodedCharacters);
		if (!_isMultiplex)
			setConstantDisplay();
	}
}

void ShiftDisplay::setAt(int section, double valueReal, char alignment) {
	setAt(section, valueReal, DEFAULT_DECIMAL_PLACES, alignment); // call other double function
}

void ShiftDisplay::setAt(int section, char value, char alignment) {
	if (section >= 0 && section < _sectionCount) { // valid section
		char originalCharacters[] = {value};
		int sectionSize = _sectionSizes[section];
		char formattedCharacters[sectionSize];
		formatCharacters(1, originalCharacters, sectionSize, formattedCharacters, alignment);
		byte encodedCharacters[sectionSize];
		encodeCharacters(sectionSize, formattedCharacters, encodedCharacters);
		modifyStorage(_sectionBegins[section], sectionSize, encodedCharacters);
		if (!_isMultiplex)
			setConstantDisplay();
	}
}

void ShiftDisplay::setAt(int section, const char value[], char alignment) {
	if (section >= 0 && section < _sectionCount) { // valid section
		int valueSize = strlen(value);
		int sectionSize = _sectionSizes[section];
		char formattedCharacters[sectionSize];
		formatCharacters(valueSize, value, sectionSize, formattedCharacters, alignment);
		byte encodedCharacters[sectionSize];
		encodeCharacters(sectionSize, formattedCharacters, encodedCharacters);
		modifyStorage(_sectionBegins[section], sectionSize, encodedCharacters);
		if (!_isMultiplex)
			setConstantDisplay();
	}
}

void ShiftDisplay::setAt(int section, const String &value, char alignment) {
	if (section >= 0 && section < _sectionCount) { // valid section
	
		// convert String to char array manually for better support between Arduino cores
		int size = 0;
		while (value[size] != '\0')
			size++;
		char str[size + 1];
		for (int i = 0; i < size; i++)
			str[i] = value[i];
		str[size] = '\0';
	
		setAt(section, str, alignment); // call char array function
	}
}

void ShiftDisplay::setAt(int section, const byte customs[]) {
	if (section >= 0 && section < _sectionCount) { // valid section
		int sectionSize = _sectionSizes[section];
		modifyStorage(_sectionBegins[section], sectionSize, customs);
		if (!_isMultiplex)
			setConstantDisplay();
	}
}

void ShiftDisplay::setAt(int section, const char characters[], bool dots[]) {
	if (section >= 0 && section < _sectionCount) { // valid section
		int sectionSize = _sectionSizes[section];
		byte encodedCharacters[sectionSize];
		encodeCharacters(sectionSize, characters, encodedCharacters);
		int begin = _sectionBegins[section];
		modifyStorage(begin, sectionSize, encodedCharacters);
		for (int i = 0; i < sectionSize; i++)
			modifyStorageDot(i+begin, dots[i]);
		if (!_isMultiplex)
			setConstantDisplay();
	}
}

void ShiftDisplay::setDotAt(int section, int relativeIndex, bool dot) {
	if (section >= 0 && section < _sectionCount) { // valid section
		if (relativeIndex >= 0 && relativeIndex < _sectionSizes[section]) { // valid index in display
			int index = _sectionBegins[section] + relativeIndex;
			modifyStorageDot(index, dot);
			if (!_isMultiplex)
				setConstantDisplay();
		}
	}
}

void ShiftDisplay::setCustomAt(int section, int relativeIndex, byte custom) {
	if (section >= 0 && section < _sectionCount) { // valid section
		if (relativeIndex >= 0 && relativeIndex < _sectionSizes[section]) { // valid index in display
			int index = _sectionBegins[section] + relativeIndex;
			modifyStorage(index, custom);
			if (!_isMultiplex)
				setConstantDisplay();
		}
	}
}

void ShiftDisplay::clear() {
	if (!_isMultiplex)
		clearConstantDisplay();
}

void ShiftDisplay::show() {
	if (_isMultiplex) {
		showMultiplexDisplay();
		clearMultiplexDisplay();
	}
}

void ShiftDisplay::show(unsigned long time) {
	if (_isMultiplex) {
		unsigned long beforeLast = millis() + time - (POV * _displaySize); // start + total - last iteration
		while (millis() <= beforeLast) // it will not enter loop if it would overtake time
			showMultiplexDisplay();
		clearMultiplexDisplay();
	}
}

void ShiftDisplay::show(int value, unsigned long time, char alignment) {
	if (_isMultiplex) {
		set(value, alignment);
		show(time);
	}
}

void ShiftDisplay::show(long value, unsigned long time, char alignment) {
	if (_isMultiplex) {
		set(value, alignment);
		show(time);
	}
}

void ShiftDisplay::show(double valueReal, unsigned long time, int decimalPlaces, char alignment) {
	if (_isMultiplex) {
		set(valueReal, decimalPlaces, alignment);
		show(time);
	}
}

void ShiftDisplay::show(double valueReal, unsigned long time, char alignment) {
	if (_isMultiplex) {
		set(valueReal, alignment);
		show(time);
	}
}

void ShiftDisplay::show(char value, unsigned long time, char alignment) {
	if (_isMultiplex) {
		set(value, alignment);
		show(time);
	}
}

void ShiftDisplay::show(const char value[], unsigned long time, char alignment) {
	if (_isMultiplex) {
		set(value, alignment);
		show(time);
	}
}

void ShiftDisplay::show(const String &value, unsigned long time, char alignment) {
	if (_isMultiplex) {
		set(value, alignment);
		show(time);
	}
}

void ShiftDisplay::show(const byte customs[], unsigned long time) {
	if (_isMultiplex) {
		set(customs);
		show(time);
	}
}

void ShiftDisplay::show(const char characters[], bool dots[], unsigned long time) {
	if (_isMultiplex) {
		set(characters, dots);
		show(time);
	}
}

void ShiftDisplay::insertPoint(int index) { setDot(index, true); }
void ShiftDisplay::removePoint(int index) { setDot(index, false); }
void ShiftDisplay::insertDot(int index) { setDot(index, true); }
void ShiftDisplay::removeDot(int index) { setDot(index, false); }
void ShiftDisplay::print(long time, int value, char alignment) { show(value, time, alignment); }
void ShiftDisplay::print(long time, long value, char alignment) { show(value, time, alignment); }
void ShiftDisplay::print(long time, double value, int decimalPlaces, char alignment) { show(value, time, decimalPlaces, alignment); }
void ShiftDisplay::print(long time, double value, char alignment) { show(value, time, alignment); }
void ShiftDisplay::print(long time, char value, char alignment) { show(value, time, alignment); }
void ShiftDisplay::print(long time, const char value[], char alignment) { show(value, time, alignment); }
void ShiftDisplay::print(long time, const String &value, char alignment) { show(value, time, alignment); }

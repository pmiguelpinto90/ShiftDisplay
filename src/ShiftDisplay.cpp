/*
ShiftDisplay
by MiguelPynto
Arduino library for driving 7-segment displays using shift registers
https://miguelpynto.github.io/ShiftDisplay/
*/

#include "Arduino.h"
#include "ShiftDisplay.h"
#include "CharacterEncoding.h"

// CONSTRUCTORS ****************************************************************

ShiftDisplay::ShiftDisplay(DisplayType displayType, int displaySize, DisplayDrive displayDrive) {
	int sectionSizes[] = {displaySize}; // single section with size of display
	construct(DEFAULT_LATCH_PIN, DEFAULT_CLOCK_PIN, DEFAULT_DATA_PIN, displayType, 1, sectionSizes, displayDrive);
}

ShiftDisplay::ShiftDisplay(int latchPin, int clockPin, int dataPin, DisplayType displayType, int displaySize, DisplayDrive displayDrive) {
	int sectionSizes[] = {displaySize}; // single section with size of display
	construct(latchPin, clockPin, dataPin, displayType, 1, sectionSizes, displayDrive);
}

ShiftDisplay::ShiftDisplay(DisplayType displayType, int sectionCount, const int sectionSizes[], DisplayDrive displayDrive) {
	construct(DEFAULT_LATCH_PIN, DEFAULT_CLOCK_PIN, DEFAULT_DATA_PIN, displayType, sectionCount, sectionSizes, displayDrive);
}

ShiftDisplay::ShiftDisplay(int latchPin, int clockPin, int dataPin, DisplayType displayType, int sectionCount, const int sectionSizes[], DisplayDrive displayDrive) {
	construct(latchPin, clockPin, dataPin, displayType, sectionCount, sectionSizes, displayDrive);
}

void ShiftDisplay::construct(int latchPin, int clockPin, int dataPin, DisplayType displayType, int sectionCount, const int sectionSizes[], DisplayDrive displayDrive) {

	// initialize pins
	_latchPin = latchPin;
	_clockPin = clockPin;
	_dataPin = dataPin;
	pinMode(_latchPin, OUTPUT);
	pinMode(_clockPin, OUTPUT);
	pinMode(_dataPin, OUTPUT);

	// initialize globals
	_isCathode = displayType == COMMON_CATHODE;
	_isMultiplexed = displayDrive == MULTIPLEXED_DRIVE;
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

	// clear cache and display
	byte empty = _isCathode ? EMPTY : ~EMPTY;
	memset(_cache, empty, MAX_DISPLAY_SIZE);
	clear();
}

// PRIVATE FUNCTIONS ***********************************************************

void ShiftDisplay::updateMultiplexedDisplay() {
	for (int i = 0; i < _displaySize; i++) {
		digitalWrite(_latchPin, LOW);

		// data for last shift register
		byte out = _isCathode ? ~INDEXES[i] : INDEXES[i];
		shiftOut(_dataPin, _clockPin, LSBFIRST, out);

		// data for first shift register
		shiftOut(_dataPin, _clockPin, LSBFIRST, _cache[i]);

		digitalWrite(_latchPin, HIGH);

		delay(POV);
	}
}

void ShiftDisplay::updateStaticDisplay() {
	digitalWrite(_latchPin, LOW);
	for (int i = _displaySize - 1; i >= 0 ; i--)
		shiftOut(_dataPin, _clockPin, LSBFIRST, _cache[i]);
	digitalWrite(_latchPin, HIGH);
}

void ShiftDisplay::clearMultiplexedDisplay() {
	digitalWrite(_latchPin, LOW);
	shiftOut(_dataPin, _clockPin, MSBFIRST, EMPTY); // 0 at both ends of led
	shiftOut(_dataPin, _clockPin, MSBFIRST, EMPTY);
	digitalWrite(_latchPin, HIGH);
}

void ShiftDisplay::clearStaticDisplay() {
	digitalWrite(_latchPin, LOW);
	byte empty = _isCathode ? EMPTY : ~EMPTY;
	for (int i = 0; i < _displaySize; i++)
		shiftOut(_dataPin, _clockPin, MSBFIRST, empty);
	digitalWrite(_latchPin, HIGH);
}

void ShiftDisplay::modifyCache(int index, byte code) {
	_cache[index] = _isCathode ? code : ~code;
}

void ShiftDisplay::modifyCache(int beginIndex, int size, const byte codes[]) {
	for (int i = 0; i < size; i++)
		_cache[i+beginIndex] = _isCathode ? codes[i] : ~codes[i];
}

void ShiftDisplay::modifyCacheDot(int index, bool dot) {
	bool bit = _isCathode ? dot : !dot;
	bitWrite(_cache[index], 0, bit);
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

int ShiftDisplay::formatCharacters(int inSize, const char input[], int outSize, char output[], char alignment, bool leadingZeros = false, int decimalPlaces = -1) {
	
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
		output[i] = leadingZeros ? '0' : ' ';
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

void ShiftDisplay::setInteger(long value, bool leadingZeros, char alignment, int section = 0) {
	int valueSize = countCharacters(value);
	char originalCharacters[valueSize];
	getCharacters(value, valueSize, originalCharacters);
	int sectionSize = _sectionSizes[section];
	char formattedCharacters[sectionSize];
	formatCharacters(valueSize, originalCharacters, sectionSize, formattedCharacters, alignment, leadingZeros);
	byte encodedCharacters[sectionSize];
	encodeCharacters(sectionSize, formattedCharacters, encodedCharacters);
	modifyCache(_sectionBegins[section], sectionSize, encodedCharacters);
}

void ShiftDisplay::setReal(double value, int decimalPlaces, bool leadingZeros, char alignment, int section = 0) {

	// if no decimal places, call integer function instead
	if (decimalPlaces == 0) {
		long value = round(valueReal);
		setInteger(value, leadingZeros, alignment, section);
		return;
	}

	// calculate value with specified decimal places as integer (eg 1.236, 2 = 124)
	long value = round(valueReal * pow(10, decimalPlaces));

	int valueSize = countCharacters(valueReal) + decimalPlaces;
	char originalCharacters[valueSize];
	getCharacters(value, valueSize, originalCharacters);
	int sectionSize = _sectionSizes[section];
	char formattedCharacters[sectionSize];
	int dotIndex = formatCharacters(valueSize, originalCharacters, sectionSize, formattedCharacters, alignment, false, decimalPlaces);
	byte encodedCharacters[sectionSize];
	encodeCharacters(sectionSize, formattedCharacters, encodedCharacters, dotIndex);
	modifyCache(_sectionBegins[section], sectionSize, encodedCharacters);
}

void ShiftDisplay::setChar(char value, char alignment, int section = 0) {
	char originalCharacters[] = {value};
	int sectionSize = _sectionSizes[section];
	char formattedCharacters[sectionSize];
	formatCharacters(1, originalCharacters, sectionSize, formattedCharacters, alignment);
	byte encodedCharacters[sectionSize];
	encodeCharacters(sectionSize, formattedCharacters, encodedCharacters);
	modifyCache(_sectionBegins[section], sectionSize, encodedCharacters);
}

void ShiftDisplay::setCharArray(const char value[], char alignment, int section = 0) {
	int valueSize = strlen(value);
	int sectionSize = _sectionSizes[section];
	char formattedCharacters[sectionSize];
	formatCharacters(valueSize, value, sectionSize, formattedCharacters, alignment);
	byte encodedCharacters[sectionSize];
	encodeCharacters(sectionSize, formattedCharacters, encodedCharacters);
	modifyCache(_sectionBegins[section], sectionSize, encodedCharacters);
}

void ShiftDisplay::setString(const String &value, char alignment, int section = 0) {
	
	// convert String to char array manually for better support between Arduino cores
	int size = 0;
	while (value[size] != '\0')
		size++;
	char str[size + 1];
	for (int i = 0; i < size; i++)
		str[i] = value[i];
	str[size] = '\0';

	setCharArray(str, alignment, section); // call char array function
}

bool ShiftDisplay::isValidSection(int section) {
	return section >= 0 && section < _sectionCount;
}

// PUBLIC FUNCTIONS ************************************************************

void ShiftDisplay::set(int value, bool leadingZeros, char alignment) {
	setInteger((long) value, leadingZeros, alignment);
}

void ShiftDisplay::set(int value, char alignment) {
	setInteger((long) value, DEFAULT_LEADING_ZEROS, alignment);
}

void ShiftDisplay::set(long value, bool leadingZeros, char alignment) {
	setInteger(value, leadingZeros, alignment);
}

void ShiftDisplay::set(long value, char alignment) {
	setInteger(value, DEFAULT_LEADING_ZEROS, alignment);
}

void ShiftDisplay::set(double valueReal, int decimalPlaces, bool leadingZeros, char alignment) {
	setReal(valueReal, decimalPlaces, leadingZeros, alignment);
}

void ShiftDisplay::set(double valueReal, char alignment) {
	setReal(valueReal, DEFAULT_DECIMAL_PLACES, DEFAULT_LEADING_ZEROS, alignment);
}

void ShiftDisplay::set(char value, char alignment) {
	setChar(value, alignment);
}

void ShiftDisplay::set(const char value[], char alignment) {
	setCharArray(value, alignment);
}

void ShiftDisplay::set(const String &value, char alignment) {
	setString(value, alignment);
}

void ShiftDisplay::set(const byte customs[]) {
	setAt(0, customs);
}

void ShiftDisplay::set(const char characters[], const bool dots[]) {
	setAt(0, characters, dots);
}

void ShiftDisplay::setAt(int section, int value, bool leadingZeros, char alignment) {
	if (isValidSection(section))
		setInteger((long) value, leadingZeros, alignment, section);
}

void ShiftDisplay::setAt(int section, int value, char alignment) {
	if (isValidSection(section))
		setInteger((long) value, DEFAULT_LEADING_ZEROS, alignment, section);
}

void ShiftDisplay::setAt(int section, long value, bool leadingZeros, char alignment) {
	if (isValidSection(section))
		setInteger(value, leadingZeros, alignment, section);
}

void ShiftDisplay::setAt(int section, long value, char alignment) {
	if (isValidSection(section))
		setInteger(value, DEFAULT_LEADING_ZEROS, alignment, section);
}

void ShiftDisplay::setAt(int section, double valueReal, int decimalPlaces, bool leadingZeros, char alignment) {
	if (isValidSection(section))
		setReal(valueReal, decimalPlaces, leadingZeros, alignment, section);
}

void ShiftDisplay::setAt(int section, double valueReal, char alignment) {
	if (isValidSection(section))
		setReal(valueReal, DEFAULT_DECIMAL_PLACES, DEFAULT_LEADING_ZEROS, alignment, section);
}

void ShiftDisplay::setAt(int section, char value, char alignment) {
	if (isValidSection(section))
		setChar(value, alignment, section);
}

void ShiftDisplay::setAt(int section, const char value[], char alignment) {
	if (isValidSection(section))
		setCharArray(value, alignment, section);
}

void ShiftDisplay::setAt(int section, const String &value, char alignment) {
	if (isValidSection(section))
		setString(value, alignment, section);
}

void ShiftDisplay::setAt(int section, const byte customs[]) {
	if (isValidSection(section)) {
		int sectionSize = _sectionSizes[section];
		modifyCache(_sectionBegins[section], sectionSize, customs);
	}
}

void ShiftDisplay::setAt(int section, const char characters[], const bool dots[]) {
	if (isValidSection(section)) {
		int sectionSize = _sectionSizes[section];
		byte encodedCharacters[sectionSize];
		encodeCharacters(sectionSize, characters, encodedCharacters);
		int begin = _sectionBegins[section];
		modifyCache(begin, sectionSize, encodedCharacters);
		for (int i = 0; i < sectionSize; i++)
			modifyCacheDot(i+begin, dots[i]);
	}
}

void ShiftDisplay::setDot(int index, bool dot) {
	setDotAt(0, index, dot);
}

void ShiftDisplay::setCustom(int index, byte custom) {
	setCustomAt(0, index, custom);
}

void ShiftDisplay::setDotAt(int section, int relativeIndex, bool dot) {
	if (isValidSection(section)) {
		if (relativeIndex >= 0 && relativeIndex < _sectionSizes[section]) { // valid index in display
			int index = _sectionBegins[section] + relativeIndex;
			modifyCacheDot(index, dot);
		}
	}
}

void ShiftDisplay::setCustomAt(int section, int relativeIndex, byte custom) {
	if (isValidSection(section)) {
		if (relativeIndex >= 0 && relativeIndex < _sectionSizes[section]) { // valid index in display
			int index = _sectionBegins[section] + relativeIndex;
			modifyCache(index, custom);
		}
	}
}

void ShiftDisplay::update() {
	if (_isMultiplexed)
		updateMultiplexedDisplay();
	else
		updateStaticDisplay();
}

void ShiftDisplay::clear() {
	if (_isMultiplexed)
		clearMultiplexedDisplay();
	else
		clearStaticDisplay();
}

void ShiftDisplay::show(unsigned long time) {
	if (_isMultiplexed) {
		unsigned long beforeLast = millis() + time - (POV * _displaySize); // start + total - last iteration
		while (millis() <= beforeLast) // it will not enter loop if it would overtake time
			updateMultiplexedDisplay();
		clearMultiplexedDisplay();
	} else {
		updateStaticDisplay();
		delay(time);
		clearStaticDisplay();
	}
}

// DEPRECATED ******************************************************************
void ShiftDisplay::insertPoint(int index) { modifyCacheDot(index, true); }
void ShiftDisplay::removePoint(int index) { modifyCacheDot(index, false); }
void ShiftDisplay::insertDot(int index) { modifyCacheDot(index, true); }
void ShiftDisplay::removeDot(int index) { modifyCacheDot(index, false); }
void ShiftDisplay::print(long time, int value, char alignment) { show(value, time, alignment); }
void ShiftDisplay::print(long time, long value, char alignment) { show(value, time, alignment); }
void ShiftDisplay::print(long time, double value, int decimalPlaces, char alignment) { show(value, time, decimalPlaces, alignment); }
void ShiftDisplay::print(long time, double value, char alignment) { show(value, time, alignment); }
void ShiftDisplay::print(long time, char value, char alignment) { show(value, time, alignment); }
void ShiftDisplay::print(long time, const char value[], char alignment) { show(value, time, alignment); }
void ShiftDisplay::print(long time, const String &value, char alignment) { show(value, time, alignment); }
void ShiftDisplay::show() { updateMultiplexedDisplay(); clearMultiplexedDisplay(); }
void ShiftDisplay::show(int value, unsigned long time, char alignment) { set(value, alignment); show(time); }
void ShiftDisplay::show(long value, unsigned long time, char alignment) { set(value, alignment); show(time); }
void ShiftDisplay::show(double valueReal, unsigned long time, int decimalPlaces, char alignment) { set(valueReal, decimalPlaces, alignment); show(time); }
void ShiftDisplay::show(double valueReal, unsigned long time, char alignment) { set(valueReal, alignment); show(time); }
void ShiftDisplay::show(char value, unsigned long time, char alignment) { set(value, alignment); show(time); }
void ShiftDisplay::show(const char value[], unsigned long time, char alignment) { set(value, alignment); show(time); }
void ShiftDisplay::show(const String &value, unsigned long time, char alignment) { set(value, alignment); show(time); }
void ShiftDisplay::show(const byte customs[], unsigned long time) { set(customs); show(time); }
void ShiftDisplay::show(const char characters[], const bool dots[], unsigned long time) { set(characters, dots); show(time); }

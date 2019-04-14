/*
ShiftDisplaySPI
by Tatu Wikman
Arduino library for driving 7-segment displays using shift registers
https://miguelpynto.github.io/ShiftDisplay/
*/

#include "Arduino.h"
#include "ShiftDisplaySPI.h"
#include "CharacterEncoding.h"
#include "SPI.h"

// Set force clear. If this is active we will flush the shift register with zeroes before sending the real data.
// Seems to be required for some specific displays.
void ShiftDisplaySPI::setForceClear(bool force)
{
	_forceClear = force;
}

void ShiftDisplaySPI::update()
{
	if (_isMultiplexed)
		updateMultiplexedDisplay();
	else
		updateStaticDisplay();
}

void ShiftDisplaySPI::clear()
{
	if (_isMultiplexed)
		clearMultiplexedDisplay();
	else
		clearStaticDisplay();
}

void ShiftDisplaySPI::show(unsigned long time) {
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

// Initialize SPI and clear the display
void ShiftDisplaySPI::begin()
{
	SPI.begin();
	SPI.setClockDivider(SPI_CLOCK_DIV2);
	SPI.setBitOrder(LSBFIRST);
	digitalWrite(_latchPin, LOW);
	digitalWrite(_ssPin, HIGH);
	clear();
}

void ShiftDisplaySPI::updateMultiplexedDisplay()
{
	for (int i = 0; i < _displaySize; i++)
	{
		// for some strange reason my robotdyn 6 digit 7 segment
		// will show garbage if it is not cleared before sending new data in...
		if (_forceClear)
		{
			clear();
		}

		digitalWrite(_ssPin, LOW);
		digitalWrite(_latchPin, LOW);

		if (_isSwapped)
		{
			SPI.transfer(_cache[i]);
			SPI.transfer(_indexes[i]);
		}
		else
		{
			SPI.transfer(_indexes[i]);
			SPI.transfer(_cache[i]);
		}

		digitalWrite(_latchPin, HIGH);
		digitalWrite(_ssPin, HIGH);
		delay(POV);
	}
}

void ShiftDisplaySPI::updateStaticDisplay()
{
	digitalWrite(_ssPin, LOW);
	digitalWrite(_latchPin, LOW);
	for (int i = _displaySize - 1; i >= 0; i--)
		SPI.transfer(_cache[i]);
	digitalWrite(_latchPin, HIGH);
	digitalWrite(_ssPin, HIGH);
}

void ShiftDisplaySPI::clearMultiplexedDisplay()
{
	byte empty = _isCathode ? EMPTY : ~EMPTY;
	digitalWrite(_ssPin, LOW);
	digitalWrite(_latchPin, LOW);

	if (_isSwapped)
	{
		SPI.transfer(empty);
		SPI.transfer(empty);
	}
	else
	{
		SPI.transfer(empty);
		SPI.transfer(empty);
	}

	digitalWrite(_latchPin, HIGH);
	digitalWrite(_ssPin, HIGH);
}

void ShiftDisplaySPI::clearStaticDisplay()
{
	digitalWrite(_ssPin, LOW);
	digitalWrite(_latchPin, LOW);
	byte empty = _isCathode ? EMPTY : ~EMPTY;
	for (int i = 0; i < _displaySize; i++)
		SPI.transfer(empty);
	digitalWrite(_latchPin, HIGH);
	digitalWrite(_ssPin, HIGH);
}
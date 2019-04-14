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

void ShiftDisplaySPI::update() {
	if (_isMultiplexed)
		updateMultiplexedDisplay();
	else
		updateStaticDisplay();
}

void ShiftDisplaySPI::begin()
{
	SPI.begin();
	SPI.setClockDivider(SPI_CLOCK_DIV2);
	SPI.setBitOrder(LSBFIRST);
	digitalWrite(_latchPin, LOW);
	clearMultiplexedDisplay();
}

void ShiftDisplaySPI::updateMultiplexedDisplay()
{
	for (int i = 0; i < _displaySize; i++)
	{
		clearMultiplexedDisplay();

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

		delay(POV);
	}
}

void ShiftDisplaySPI::updateStaticDisplay()
{
	digitalWrite(_latchPin, LOW);
	for (int i = _displaySize - 1; i >= 0; i--)
		SPI.transfer(_cache[i]);
	digitalWrite(_latchPin, HIGH);
}

void ShiftDisplaySPI::clearMultiplexedDisplay()
{
	digitalWrite(_latchPin, LOW);

	if (_isSwapped)
	{
		SPI.transfer(0b11111111);
		SPI.transfer(0b00000000);
	}
	else
	{
		SPI.transfer(0b00000000);
		SPI.transfer(0b11111111);
	}

	digitalWrite(_latchPin, HIGH);
}
/*
ShiftDisplaySPI
by Tatu Wikman
Arduino library for driving 7-segment displays using shift registers
https://miguelpynto.github.io/ShiftDisplay/
*/

#ifndef ShiftDisplaySPI_h
#define ShiftDisplaySPI_h
#include "Arduino.h"
#include "SPI.h"
#include "ShiftDisplay.h"

class ShiftDisplaySPI : public ShiftDisplay
{

  protected:
	int _ssPin;

  public:
	ShiftDisplaySPI(int ssPin, int latchPin, DisplayType displayType, int displaySize, bool swappedShiftRegisters, DisplayDrive displayDrive, const int indexes[])
		: ShiftDisplay(displayType, displaySize, swappedShiftRegisters, indexes) {
			_ssPin = ssPin;
			_latchPin = latchPin;
		  };

	void begin();

	void updateMultiplexedDisplay();
	void updateStaticDisplay();
	void clearMultiplexedDisplay();

	// why I have to override this???
	// shouldn't it be enought to override the updateMultiplixed and updateStaticDisplay methods?
	void update();
};

#endif

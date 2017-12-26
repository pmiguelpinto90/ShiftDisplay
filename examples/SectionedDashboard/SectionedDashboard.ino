/*
ShiftDisplay example
by MiguelPynto
Car dashboard with 3 sections
https://miguelpynto.github.io/ShiftDisplay/
*/

#include <ShiftDisplay.h>

enum sections { // in order of connection
	SPEEDOMETER,
	ODOMETER,
	FUEL_GAUGE
};
const int SECTION_SIZES[] = {3, 4, 1, 0};
const int RESERVE = 0; // digit index relative to FUEL_GAUGE

ShiftDisplay dashboard(COMMON_CATHODE, SECTION_SIZES);

int getSpeed() {
	return 120;
}

int getDistance() {
	return 4571;
}

char getFuel() {
	return 'E';
}

bool inReserve() {
	return true;
}

void setup() {
	dashboard.set("lets go");
	while (millis() < 2000 || getSpeed() == 0)
		dashboard.update();
}

void loop() {
	dashboard.setAt(SPEEDOMETER, getSpeed());
	dashboard.setAt(ODOMETER, getDistance());
	dashboard.setAt(FUEL_GAUGE, getFuel());
	dashboard.changeDotAt(FUEL_GAUGE, RESERVE, inReserve());
	dashboard.show(1000);
}

/*
ShiftDisplay example
by MiguelPynto
Simulate a weather station, showing info such as temperature and condition
https://miguelpynto.github.io/ShiftDisplay/
*/

#include <ShiftDisplay.h>

const int LATCH_PIN = 6;
const int CLOCK_PIN = 7;
const int DATA_PIN = 5;
const DisplayType DISPLAY_TYPE = COMMON_ANODE; // COMMON_CATHODE or COMMON_ANODE
const int DISPLAY_SIZE = 8; // number of digits on display

ShiftDisplay display(LATCH_PIN, CLOCK_PIN, DATA_PIN, DISPLAY_TYPE, DISPLAY_SIZE);

float getTemperature() {
	delay(1000); // simulate processing
	return 21.36;
}

String getCondition() {
	delay(1000); // simulate processing
	return "Sunny";
}

void setup() {
}

void loop() {
	float temp = getTemperature();
	String condition = getCondition();
	condition = "        " + condition; // prefix with spaces

	// for 4 seconds, show temperature with one decimal, aligned to center of display
	display.set(temp, 1, ALIGN_CENTER);
	display.show(4000);

	// show condition with marquee effect
	while (condition.length() > 0) {
		display.set(condition, ALIGN_LEFT);
		display.show(500);
		condition.remove(0,1);
	}
}

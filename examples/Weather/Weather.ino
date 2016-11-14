#include <ShiftDisplay.h>

const int LATCH_PIN = 6;
const int CLOCK_PIN = 7;
const int DATA_PIN = 5;
const int DISPLAY_TYPE = COMMON_ANODE; // COMMON_CATHODE or COMMON_ANODE
const int DISPLAY_SIZE = 8; // number of digits on display

ShiftDisplay disp(LATCH_PIN, CLOCK_PIN, DATA_PIN, DISPLAY_TYPE, DISPLAY_SIZE);

float getTemperature() {
	delay(1000);
	return 21.36;
}

String getCondition() {
	delay(1000);
	return "Sunny";
}

void setup() {
}

void loop() {
	float temp = getTemperature();
	String condition = getCondition();
	condition = "        " + condition;

	// print temperature with one decimal number, aligned to center of display, for 4 seconds
	disp.print(4000, temp, 1, ALIGN_CENTER);

	// print string with marquee effect
	while (condition.length() > 0) {
		disp.print(500, condition, ALIGN_LEFT);
		condition.remove(0,1);
	}
}

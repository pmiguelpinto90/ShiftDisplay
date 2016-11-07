#include <ShiftDisplay.h>

const int LATCH_PIN = 6;
const int CLOCK_PIN = 7;
const int DATA_PIN = 5;
const int DISPLAY_TYPE = COMMON_CATHODE;
const int DISPLAY_SIZE = 3; // number of digits
//const int BUTTON_PIN = 2;

ShiftDisplay disp(LATCH_PIN, CLOCK_PIN, DATA_PIN, DISPLAY_TYPE, DISPLAY_SIZE);

void setup() {
	for (int i = 600; i > 0; i--)
		disp.print(100, i);
	disp.set(0);
}

void loop() {
	disp.show();
}

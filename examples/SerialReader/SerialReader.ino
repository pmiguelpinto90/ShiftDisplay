#include <ShiftDisplay.h>
#include <Wire.h>

const int LATCH_PIN = 6;
const int CLOCK_PIN = 7;
const int DATA_PIN = 5;
const int DISPLAY_TYPE = COMMON_CATHODE; // COMMON_CATHODE or COMMON_ANODE
const int DISPLAY_SIZE = 6; // number of digits

ShiftDisplay disp(LATCH_PIN, CLOCK_PIN, DATA_PIN, DISPLAY_TYPE, DISPLAY_SIZE);

void receiveEvent(int numBytes) {
	String input;
	while (Wire.available())
		input += Wire.read();
	disp.set(input);
}

void setup() {
	Wire.begin(8);
	Wire.onReceive(receiveEvent);
}

void loop() {
	disp.show();
}

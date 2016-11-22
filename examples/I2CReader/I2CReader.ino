// https://github.com/MiguelPynto/ShiftDisplay

#include <ShiftDisplay.h>
#include <Wire.h>

const int LATCH_PIN = 6;
const int CLOCK_PIN = 7;
const int DATA_PIN = 5;
const int DISPLAY_TYPE = COMMON_CATHODE; // COMMON_CATHODE or COMMON_ANODE
const int DISPLAY_SIZE = 6; // number of digits on display

ShiftDisplay led(LATCH_PIN, CLOCK_PIN, DATA_PIN, DISPLAY_TYPE, DISPLAY_SIZE);

void receiveEvent(int numBytes) {
	char text[5]; // string length is 4 plus \0
	for (int i = 0; i < 4; i++)
		text[i] = Wire.read();
	text[4] = '\0';
	int alignment = Wire.read(); // last byte from received data is alignment
	led.set(text, alignment);
}

void setup() {
	Wire.begin(8);
	Wire.onReceive(receiveEvent);
}

void loop() {
	led.show();
}

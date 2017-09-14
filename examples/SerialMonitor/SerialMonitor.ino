/*
ShiftDisplay example
by MiguelPynto
Print strings from Serial Monitor (250000 baud) on display
https://miguelpynto.github.io/ShiftDisplay/
*/

#include <ShiftDisplay.h>

const int LATCH_PIN = 6;
const int CLOCK_PIN = 7;
const int DATA_PIN = 5;
const int DISPLAY_TYPE = COMMON_ANODE; // COMMON_CATHODE or COMMON_ANODE
const int DISPLAY_SIZE = 8; // number of digits on display

ShiftDisplay display(LATCH_PIN, CLOCK_PIN, DATA_PIN, DISPLAY_TYPE, DISPLAY_SIZE);

void readSerial() {
	char input[Serial.available() + 1]; // str len + NULL
	int i = 0;
	while (Serial.available() > 0) {
		char c = Serial.read();
		if (c == '\n' || c == '\0')
			break; // exit while
		else
			input[i++] = c;
	}
	input[i] = '\0'; // NULL terminate string
	display.set(input); // save
}

void setup() {
	Serial.begin(250000);
}

void loop() {
	if (Serial.available() > 0)
		readSerial();
	display.show(); // show last read from serial
}

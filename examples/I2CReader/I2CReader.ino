/*
ShiftDisplay example
by MiguelPynto
Receive pre-formatted data over I2C
https://miguelpynto.github.io/ShiftDisplay/
*/

#include <ShiftDisplay.h>
#include <Wire.h>
// connect pin A4 to other Arduino pin A4, pin A5 to other Arduino pin A5, and GND to other Arduino GND

const int DISPLAY_ADDRESS = 8;
const int LATCH_PIN = 6;
const int CLOCK_PIN = 7;
const int DATA_PIN = 5;
const int DISPLAY_TYPE = COMMON_CATHODE; // COMMON_CATHODE or COMMON_ANODE
const int DISPLAY_SIZE = 4; // number of digits on display

ShiftDisplay display(LATCH_PIN, CLOCK_PIN, DATA_PIN, DISPLAY_TYPE, DISPLAY_SIZE);

void receiveEvent(int numBytes) {
	char str[numBytes];
	for (int i = 0; i < numBytes; i++)
		str[i] = Wire.read();
	display.set(str); // save to buffer
}

void setup() {
	Wire.begin(DISPLAY_ADDRESS); // this is slave
	Wire.onReceive(receiveEvent);
}

void loop() {
	display.show(); // show what is on buffer forever
}

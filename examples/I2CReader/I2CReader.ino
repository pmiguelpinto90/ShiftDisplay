/*
ShiftDisplay example
by Miguel Pynto
Receive pre-formatted data over I2C
http://pynto.me/ShiftDisplay/
*/

#include <ShiftDisplay.h>
#include <Wire.h>
// connect pin A4 to other Arduino pin A4, and pin A5 to other Arduino pin A5

const int LATCH_PIN = 6;
const int CLOCK_PIN = 7;
const int DATA_PIN = 5;
const int DISPLAY_TYPE = COMMON_CATHODE; // COMMON_CATHODE or COMMON_ANODE
const int DISPLAY_SIZE = 6; // number of digits on display

ShiftDisplay led(LATCH_PIN, CLOCK_PIN, DATA_PIN, DISPLAY_TYPE, DISPLAY_SIZE);

void receiveEvent(int numBytes) {
	char text[5]; // string length is 4 + NULL
	for (int i = 0; i < 4; i++) // three first bytes from received data is text to print
		text[i] = Wire.read();
	text[4] = '\0'; // NULL-terminate string
	char alignment = Wire.read(); // last byte from received data is alignment
	led.set(text, alignment); // save to buffer
}

void setup() {
	Wire.begin(8); // this is slave with adress 8
	Wire.onReceive(receiveEvent);
}

void loop() {
	led.show(); // always showing whats on buffer
}

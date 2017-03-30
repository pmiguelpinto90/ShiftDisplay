/*
ShiftDisplay example
by MiguelPynto
Count minutes passing by
https://miguelpynto.github.io/ShiftDisplay/
*/

#include <ShiftDisplay.h>

// Arduino pin 6 connected to shift register latch, pin 7 to clock and pin 5 to data
// common anode display with 2 digits
ShiftDisplay led(6, 7, 5, COMMON_ANODE, 2);

int m;

void setup() {
	m = 0;
}

void loop() {
	led.set(m, ALIGN_RIGHT);
	for (int i = 0; i < 60; i++) {
		led.insertPoint(1);
		led.show(500);
		led.removePoint(1);
		led.show(500);
	}
	m++;
}

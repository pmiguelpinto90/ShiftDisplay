/*
ShiftDisplay example
by MiguelPynto
Count minutes passing by
https://miguelpynto.github.io/ShiftDisplay/
*/

#include <ShiftDisplay.h>

// Arduino pin 6 connected to shift register latch, pin 7 to clock and pin 5 to data
// common anode display with 2 digits
ShiftDisplay sd(6, 7, 5, COMMON_ANODE, 2);

void setup() {}

void loop() {
	static int m = 0;
	sd.set(m, ALIGN_LEFT);
	for (int i = 0; i < 60; i++) {
		int pos = m < 10 ? 0 : 1;
		sd.setDot(pos, true);
		sd.show(500);
		sd.setDot(pos, false);
		sd.show(500);
	}
	m++;
}

/*
ShiftDisplay example
by MiguelPynto
Race countdown: 3.. 2.. 1.. GO!
https://miguelpynto.github.io/ShiftDisplay/
*/

#include <ShiftDisplay.h>

// Arduino pin 6 connected to shift register latch, pin 7 to clock and pin 5 to data
// common cathode display with 3 digits
ShiftDisplay sd(6, 7, 5, COMMON_CATHODE, 3);

void setup() {
	for (int i = 3; i > 0; i--) {
		sd.show(i, 400, ALIGN_CENTER); // save number and show it for 400ms
		sd.setDot(1, true); // add dot to saved number
		sd.show(400); // show number with dot for 400ms
	}
	sd.set("GO"); // save "GO" to buffer
}

void loop() {
	sd.show(); // show "GO" while in loop
}

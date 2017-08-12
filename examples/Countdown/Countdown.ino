/*
ShiftDisplay example
by MiguelPynto
Race countdown: 3.. 2.. 1.. GO!
https://miguelpynto.github.io/ShiftDisplay/
*/

#include <ShiftDisplay.h>

// Arduino pin 6 connected to shift register latch, pin 7 to clock and pin 5 to data
// common cathode display with 3 digits
ShiftDisplay led(6, 7, 5, COMMON_CATHODE, 3);

void setup() {
	for (int i = 3; i > 0; i--) {
		led.print(400, i, ALIGN_CENTER); // save number and show it for 400ms
		led.insertPoint(1); // add dot to saved number
		led.show(400); // show number with dot for 400ms
	}
	led.set("GO"); // save "GO" to buffer
}

void loop() {
	led.show(); // show "GO" while in loop
}

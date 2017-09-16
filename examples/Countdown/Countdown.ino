/*
ShiftDisplay example
by MiguelPynto
Race countdown: 3.. 2.. 1.. GO!
https://miguelpynto.github.io/ShiftDisplay/
*/

#include <ShiftDisplay.h>

ShiftDisplay display(COMMON_CATHODE, 3);

void setup() {
	for (int i = 3; i > 0; i--) {
		display.show(i, 400, ALIGN_CENTER); // store number and show it for 400ms
		display.setDot(1, true); // add dot to stored number
		display.show(400); // show number with dot for 400ms
	}
	display.set("GO"); // store "GO"
}

void loop() {
	display.show(); // show stored "GO" while in loop
}

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
		// store number and show it for 400ms
		display.set(i, ALIGN_CENTER);
		display.show(400);
		// add dot to stored number and show it for 400ms
		display.changeDot(1);
		display.show(400);
	}
	display.set("GO"); // store "GO"
}

void loop() {
	display.update(); // show stored "GO" while in loop
}

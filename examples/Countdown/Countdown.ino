/*
ShiftDisplay example
by Miguel Pynto
Race countdown: 3.. 2.. 1.. GO!
http://pynto.me/ShiftDisplay/
*/

#include <ShiftDisplay.h>

// Arduino pin 6 connected to shift register latch, pin 7 to clock and pin 5 to data
// common cathode display with 3 digits
ShiftDisplay led(6, 7, 5, COMMON_CATHODE, 3);

void setup() {
	for (int i = 3; i > 0; i--)
		led.print(800, i, ALIGN_CENTER); // save number and show it for 800ms
	led.set("GO!"); // save "GO!" to buffer
}

void loop() {
	led.show(); // show "GO!" while in loop
}

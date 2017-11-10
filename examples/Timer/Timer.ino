/*
ShiftDisplay example
by MiguelPynto
One minute clock timer, pausable with button
https://miguelpynto.github.io/ShiftDisplay/
*/

#include <ShiftDisplay.h>

const DisplayType DISPLAY_TYPE = COMMON_CATHODE; // COMMON_CATHODE or COMMON_ANODE
const int DISPLAY_SIZE = 3; // number of digits on display
const int BUTTON_PIN = 2; // connect one end of button to pin 2 and other to ground

volatile bool buttonPressed;

ShiftDisplay display(DISPLAY_TYPE, DISPLAY_SIZE);

void debounce() {
	display.show(100); // execution will delay for 100ms
}

void buttonPressInterrupt() {
	buttonPressed = true;
}

void setup() {
	pinMode(BUTTON_PIN, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonPressInterrupt, FALLING);
	buttonPressed = false;
}

void loop() {

	// start
	float n;
	for (float i = 600; i > 0; i--) {
		n = i / 10;
		if (buttonPressed) {

			// pause
			debounce();
			buttonPressed = false;
			while (!buttonPressed)
				display.update(); // show previous value (that was on display when button was pressed)

			// unpause
			debounce();
			buttonPressed = false;
		}
		display.set(n, 1); // float with one decimal place
		display.show(100);
	}

	// finished
	display.set("end");
	while (!buttonPressed)
		display.update(); // show end while button isnt pressed

	// restart
	debounce();
	buttonPressed = false;
}

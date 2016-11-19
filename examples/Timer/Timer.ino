#include <ShiftDisplay.h>

const int LATCH_PIN = 6;
const int CLOCK_PIN = 7;
const int DATA_PIN = 5;
const int DISPLAY_TYPE = COMMON_CATHODE; // COMMON_CATHODE or COMMON_ANODE
const int DISPLAY_SIZE = 3; // number of digits on display
const int BUTTON_PIN = 2;

volatile bool buttonPressed;

ShiftDisplay led(LATCH_PIN, CLOCK_PIN, DATA_PIN, DISPLAY_TYPE, DISPLAY_SIZE);

void debounce() {
	led.show(100); // execution will delay for 100ms
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
	float n;
	for (float i = 600; i > 0; i--) {
		n = i / 10;
		if (buttonPressed) {

			// pause
			debounce();
			buttonPressed = false;
			while (!buttonPressed)
				led.show(); // show previous value (that was on display when button was pressed)

			// unpause
			debounce();
			buttonPressed = false;
		}
		led.print(100, n, 1); // float with one decimal place
	}

	led.set("end");
	while (!buttonPressed)
		led.show();
	debounce();
	buttonPressed = false;
}

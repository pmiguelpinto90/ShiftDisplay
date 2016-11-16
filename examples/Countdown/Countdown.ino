#include <ShiftDisplay.h>

ShiftDisplay led(6, 7, 5, COMMON_CATHODE, 3);

void setup() {
	for (int i = 3; i > 0; i--)
		led.print(800, i);
	led.set("GO");
}

void loop() {
	led.show();
}

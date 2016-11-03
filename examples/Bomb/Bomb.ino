#include <ShiftDisplay.h>

// A common cathode display with 3 digit. Latch, clock and data
// are connected to 6, 7 and 5 pins respectively.
ShiftDisplay disp(6, 7, 5, true, 3);

void setup() {
	for (int i = 600; i > 0; i--) {
		disp.set(i);
		disp.show(100);
	}
	disp.set("pum");
}

void loop() {
	disp.refresh();
}

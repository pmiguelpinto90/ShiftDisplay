#include <ShiftDisplay.h>

// A common anode display with 4 digits. Latch, clock and data
// are connected to 6, 7 and 5 pins respectively.
ShiftDisplay disp(6, 7, 5, false, 4);

void setup() {
}

void loop() {
	String s;
	disp.set(s = "s", ALIGN_RIGHT);
	disp.show(500);
	disp.set(s = "su", ALIGN_RIGHT);
	disp.show(500);
	disp.set(s = "sun", ALIGN_RIGHT);
	disp.show(500);
	disp.set(s = "sunn", ALIGN_RIGHT);
	disp.show(500);
	disp.set(s = "unny", ALIGN_LEFT);
	disp.show(500);
	disp.set(s = "nny", ALIGN_LEFT);
	disp.show(500);
	disp.set(s = "ny", ALIGN_LEFT);
	disp.show(500);
	disp.set(s = "y", ALIGN_LEFT);
	disp.show(500);
	disp.set(s = "");
	disp.show(500);
}
#include <ShiftDisplay.h>

ShiftDisplay disp(6, 7, 5, COMMON_CATHODE, 3);

void setup() {
	for (int i = 3; i > 0; i--)
		disp.print(800, i);
	disp.set("GO");
}

void loop() {
	disp.show();
}

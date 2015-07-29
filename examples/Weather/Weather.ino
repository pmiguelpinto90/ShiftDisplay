#include <ShiftDisplay.h>

// A 4 digit common anode display. With latch, clock and data connected
// to 6, 7 and 5 pins respectively.
ShiftDisplay disp(6, 7, 5, false, 4);

void setup() {
}

void loop() {
  // disp.printMenu('t', 9, 2000);
  // disp.printMenu('h', 64, 2000);

  disp.print("c", 500, ALIGNMENT_RIGHT, ANIMATION_NONE);
  disp.print("cl", 500, ALIGNMENT_RIGHT, ANIMATION_NONE);
  disp.print("cle", 500, ALIGNMENT_RIGHT, ANIMATION_NONE);
  disp.print("clea", 500, ALIGNMENT_RIGHT, ANIMATION_NONE);
  disp.print("lear", 500, ALIGNMENT_LEFT, ANIMATION_NONE);
  disp.print("ear", 500, ALIGNMENT_LEFT, ANIMATION_NONE);
  disp.print("ar", 500, ALIGNMENT_LEFT, ANIMATION_NONE);
  disp.print("r", 500, ALIGNMENT_LEFT, ANIMATION_NONE);
  disp.print("", 500, ALIGNMENT_LEFT, ANIMATION_NONE);
}

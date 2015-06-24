#include <ShiftDisplay.h>

// A 4 digit common anode display. With latch, clock and data connected
// to 6, 7 and 5 pins respectively.
ShiftDisplay disp(6, 7, 5, false, 4);

void setup() {
}

void loop() {
  // disp.printMenu('t', 9, 2000);
  // disp.printMenu('h', 64, 2000);

  disp.print("c", 500, ANIMATION_NONE, ALIGNMENT_RIGHT);
  disp.print("cl", 500, ANIMATION_NONE, ALIGNMENT_RIGHT);
  disp.print("cle", 500, ANIMATION_NONE, ALIGNMENT_RIGHT);
  disp.print("clea", 500, ANIMATION_NONE, ALIGNMENT_RIGHT);
  disp.print("lear", 500, ANIMATION_NONE, ALIGNMENT_LEFT);
  disp.print("ear", 500, ANIMATION_NONE, ALIGNMENT_LEFT);
  disp.print("ar", 500, ANIMATION_NONE, ALIGNMENT_LEFT);
  disp.print("r", 500, ANIMATION_NONE, ALIGNMENT_LEFT);
  disp.print("", 500, ANIMATION_NONE, ALIGNMENT_LEFT);
}

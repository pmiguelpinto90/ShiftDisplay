#include <ShiftDisplay.h>

ShiftDisplay disp(6, 7, 5, true, 3);

void setup() {
}

void loop() {
  // disp.printMenu('t', 9, 2000);
  // disp.printMenu('h', 64, 2000);

  disp.print("  c", 500, ANIMATION_NONE, ALIGNMENT_LEFT);
  disp.print(" cl", 500, ANIMATION_NONE, ALIGNMENT_LEFT);
  disp.print("cle", 500, ANIMATION_NONE, ALIGNMENT_LEFT);
  disp.print("lea", 500, ANIMATION_NONE, ALIGNMENT_LEFT);
  disp.print("ear", 500, ANIMATION_NONE, ALIGNMENT_LEFT);
  disp.print("ar", 500, ANIMATION_NONE, ALIGNMENT_LEFT);
  disp.print("r", 500, ANIMATION_NONE, ALIGNMENT_LEFT);
}

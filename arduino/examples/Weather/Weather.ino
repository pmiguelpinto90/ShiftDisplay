#include <ShiftDisplay.h>

ShiftDisplay disp(6, 7, 5, true, 3);

void setup() {
}

void loop() {
  disp.printMenu('t', 9, 2000);
  disp.printMenu('h', 64, 2000);

  disp.print("  c", 500);
  disp.print(" cl", 500);
  disp.print("cle", 500);
  disp.print("lea", 500);
  disp.print("ear", 500);
  disp.print("ar", 500);
  disp.print("r", 500);
}

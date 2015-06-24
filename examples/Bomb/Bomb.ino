#include <ShiftDisplayMini.h>

// A 3 digit common cathode display. With latch, clock and data connected
// to 6, 7 and 5 pins respectively.
ShiftDisplayMini disp(6, 7, 5, true, 3);

void setup() {
  for (int i = 42; i > 0; i--)
    disp.print(i, 400);
}

void loop() {
  disp.print("pum", 1);
}

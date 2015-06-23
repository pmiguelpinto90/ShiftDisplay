#include <ShiftDisplay.h>

ShiftDisplay disp(6, 7, 5, true, 3);

void setup() {
  for (int i = 42; i > 0; i--)
    disp.print(i, 400, ANIMATION_NONE, ALIGNMENT_RIGHT);
}

void loop() {
  disp.print("hey", 1, ANIMATION_NONE, ALIGNMENT_RIGHT);
}

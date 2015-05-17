#include <ShiftDisplay.h>

ShiftDisplay disp(6, 7, 5, true, 3);

void setup() {
  for (int i = 128; i > 0; i--)
    disp.print(i, 500);
}

void loop() {
  disp.print("hey", 1);
}

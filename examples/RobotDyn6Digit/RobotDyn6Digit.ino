#include "Arduino.h"
#include "ShiftDisplaySPI.h"

// Latch pin
#define LATCHPIN 6

// Slave Select, if using multiple displays and you want to control them
// totally independent give each of them their own SS pin
// for this example we are using the same pin as LATCHPIN (as SS does the)
// same as latch pin (goes low and high the same way)
#define SSPIN 6

/* SPI will use pins 13 and 11 by default, no need to specify them
#define CLOCKPIN 13 // SPI pin is always 13 and 11
#define DATAPIN 11  // SPI pin is always 13 and 11
*/

// RobotDyn 6 digit
#define DISPLAYSIZE 6

// with swapped register
#define SWAPPED_REGISTERS true

// RobotDyn indexes are weird
const int indexes[] = {4, 5, 6, 0, 1, 2};

ShiftDisplaySPI display(SSPIN, LATCHPIN, COMMON_ANODE, DISPLAYSIZE, SWAPPED_REGISTERS, MULTIPLEXED_DRIVE, indexes);

// for demo
unsigned long previousMillis = 0;
const long interval = 1000;
long value = 0;

// demo strings to show
String texts[] = {
    "Hello",
    "World",
    "How",
    "Are",
    "You"};

void setup()
{
    // required for SPI version of the library, mainly calls SPI.begin();
    display.begin();

    // This seems to be quite specific for the RobotDyn 6 digit (or just for the
    // one monday unit I have).
    // We have to send empty data to the display everytime or it will just show garbage.
    // This sets force clear on which will send empty to the chip before sending
    // the real data during update()
    display.setForceClear(true);

    // first show a countdown
    for (int i = 9; i >= 0; i--)
    {
        // store number and show it for 400ms
        display.set(i, ALIGN_CENTER);
        display.show(400);
        display.changeDot(2); // set the second digit dot
        display.show(400);
    }
}

void loop()
{
    // rember to refresh the display often (it is multiplexed)
    display.update();

    // do your own things without blocking the loop.
    // this demo takes a string from the texts array and sets it to the display
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval)
    {
        previousMillis = currentMillis;
        display.set(texts[value++ % 5]);
    }
}

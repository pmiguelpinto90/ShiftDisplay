/*
ShiftDisplay example
by HamzaHajeir
non-blocking working example with ShiftDisplay, Working with ESP8266
*/

#include <ShiftDisplay.h>
#include <Ticker.h> 	//For timing

// ESP8266 pin D3 connected to shift register latch, pin D4 to clock and pin D5 to data
// common cathod display with 8 digits
ShiftDisplay display(D3, D4, D5, COMMON_CATHODE, 8);

Ticker displayUpdater;

void setup() {
  displayUpdater.attach_ms(1,displayUpdate);
  display.set(0);
}

void loop() {
  //Get free times here
  static uint32_t prevmillis;
  
  if(millis() - prevmillis > 1000){
		
    display.set((long)(prevmillis / 1000)%100000000);
    prevmillis = millis();
		
  }
}

ICACHE_RAM_ATTR void displayUpdate() {
  display.asyncUpdate();
}
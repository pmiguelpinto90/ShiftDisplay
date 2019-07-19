/*
ShiftDisplay example
by HamzaHajeir
non-blocking working example with ShiftDisplay, Working with ATMEGA328 Microcontrollers

*/

#include <ShiftDisplay.h>

// Arduino pin 6 connected to shift register latch, pin 7 to clock and pin 5 to data
// common cathod display with 8 digits
ShiftDisplay display(6, 7, 5, COMMON_CATHODE, 8);

void setupTimer2() {	//setup a 1khz timer interrupt
  noInterrupts();
  // Clear registers
  TCCR2A = 0;
  TCCR2B = 0;
  TCNT2 = 0;

  // 1000 Hz (16000000/((124+1)*128))
  OCR2A = 124;
  // CTC
  TCCR2A |= (1 << WGM21);
  // Prescaler 128
  TCCR2B |= (1 << CS22) | (1 << CS20);
  // Output Compare Match A Interrupt Enable
  TIMSK2 |= (1 << OCIE2A);
  interrupts();
}

void setup() {
  setupTimer2();
}

void loop() {
  //Get free times here
  static uint32_t prevmillis;
  if(millis() - prevmillis > 1000){
		
    display.set((long)(prevmillis / 1000)%100000000);
    prevmillis = millis();
		
  }
}

ISR(TIMER2_COMPA_vect) {
	
	display.asyncUpdate();
	
}
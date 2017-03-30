/*
ShiftDisplay example
by MiguelPynto
Send pre-formatted data over I2C
https://miguelpynto.github.io/ShiftDisplay/
*/

#include <ShiftDisplay.h>
#include <Wire.h>
// connect pin A4 to other Arduino pin A4, and pin A5 to other Arduino pin A5

char getSomething() {
	char c = 'A' + random(26); // random letter between A and Z
	return c;
}

int getSomethingMore() {
	int n = random(100, 1000); // random number between 100 and 999
	delay(n); // simulate processing
	return n;
}

void setup() {
	Wire.begin(); // this is master
}

void loop() {
	char letter = getSomething();
	int n = getSomethingMore();
	char number[4];
	itoa(n, number, DEC);

	Wire.beginTransmission(8);
	Wire.write(letter); // first byte sent as char
	Wire.write(number); // three bytes sent as char array, \0 is not sent
	Wire.write(ALIGN_RIGHT); // last byte sent as alignment
	Wire.endTransmission();
}

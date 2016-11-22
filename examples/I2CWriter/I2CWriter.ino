// https://github.com/MiguelPynto/ShiftDisplay

#include <ShiftDisplay.h>
#include <Wire.h>

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
	Wire.begin();
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

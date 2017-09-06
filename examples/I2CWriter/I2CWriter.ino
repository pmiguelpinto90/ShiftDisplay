/*
ShiftDisplay example
by MiguelPynto
Send pre-formatted data over I2C
https://miguelpynto.github.io/ShiftDisplay/
*/

#include <Wire.h>
// connect pin A4 to other Arduino pin A4, pin A5 to other Arduino pin A5, and GND to other Arduino GND

const int DISPLAY_ADDRESS = 8;

char getSomething() {
	char c = 'A' + random(26); // random letter between A and Z
	return c;
}

int getSomethingMore() {
	int n = random(100, 1000); // random number between 100 and 999
	delay(n); // simulate processing
	return n;
}

void format(char letter, int num, char str[]) {
	str[0] = letter;
	str[1] = '0' + (num / 100);
	str[2] = '0' + (num / 10 % 10);
	str[3] = '0' + (num % 10);
	str[4] = '\0';
}

void setup() {
	Wire.begin(); // this is master
}

void loop() {
	char letter = getSomething();
	int num = getSomethingMore();
	char str[5];
	format(letter, num, str);

	Wire.beginTransmission(DISPLAY_ADDRESS);
	Wire.write(str); // send 5 bytes as char array (string)
	Wire.endTransmission();
}

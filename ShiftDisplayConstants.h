/*
  ShiftDisplay 2.2.0 (14/07/2015)
  https://github.com/Pyntoo/ShiftDisplay
*/


#ifndef constants_h
#define constants_h


const int POV = 5; // delay for persistence of vision

// characters encoding for common cathode in MSBFIRST
const byte DIGITS[10] = {
	//GFEDCBA
	B00111111, // 0
	B00000110, // 1
	B01011011, // 2
	B01001111, // 3
	B01100110, // 4
	B01101101, // 5
	B01111101, // 6
	B00000111, // 7
	B01111111, // 8
	B01100111 // 9
};
const byte LETTERS[26] = {
	//GFEDCBA
	B01110111, // a
	B01111100, // b
	B00111001, // c
	B01011110, // d
	B01111001, // e
	B01110001, // f
	B00111101, // g
	B01110110, // h
	B00110000, // i
	B00011110, // j
	B01110101, // k
	B00111000, // l
	B00110111, // m
	B01010100, // n
	B01011100, // o
	B01110011, // p
	B01100111, // q
	B00110001, // r
	B01101101, // s
	B01111000, // t
	B00011100, // u
	B00111110, // v
	B01111110, // w
	B00110110, // x
	B01101110, // y
	B01011011 // z
};
const byte MINUS = B01000000;
const byte DOT = B10000000;
const byte BLANK = B00000000;

// displays encoding for common cathode in LSBFIRST
const byte DISPLAYS[8] = {
	B10000000,
	B01000000,
	B00100000,
	B00010000,
	B00001000,
	B00000100,
	B00000010,
	B00000001
};
const byte DISPLAYS_OFF = {
	B00000000,
};


#endif

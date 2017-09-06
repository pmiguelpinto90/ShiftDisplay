/*
ShiftDisplay
by MiguelPynto
Arduino library for driving multiple-digit 7-segment LED displays using 74HC595 shift registers
https://miguelpynto.github.io/ShiftDisplay/
*/

#ifndef CharacterEncoding_h
#define CharacterEncoding_h

// encoding for characters segments (common cathode with LSBFIRST)
const byte NUMBERS[] = {
	//ABCDEFG
	 B11111100, // 0
	 B01100000, // 1
	 B11011010, // 2
	 B11110010, // 3
	 B01100110, // 4
	 B10110110, // 5
	 B10111110, // 6
	 B11100000, // 7
	 B11111110, // 8
	 B11100110  // 9
};

const byte LETTERS[] = {
	//ABCDEFG
	 B11101110, // a
	 B00111110, // b
	 B10011100, // c
	 B01111010, // d
	 B10011110, // e
	 B10001110, // f
	 B10111100, // g
	 B01101110, // h
	 B00001100, // i
	 B01111000, // j
	 B10101110, // k
	 B00011100, // l
	 B11101100, // m
	 B00101010, // n
	 B00111010, // o
	 B11001110, // p
	 B11100110, // q
	 B10001100, // r
	 B10110110, // s
	 B00011110, // t
	 B00111000, // u
	 B01111100, // v
	 B01111110, // w
	 B01101100, // x
	 B01110110, // y
	 B11011010 // z
};

const byte MINUS = B00000010;
const byte BLANK = B00000000;

// encoding for each index in display (common anode with LSBFIRST)
const byte INDEXES[] = {
	B10000000, // 0
	B01000000, // 1
	B00100000, // 2
	B00010000, // 3
	B00001000, // 4
	B00000100, // 5
	B00000010, // 6
	B00000001 // 7
};

#endif

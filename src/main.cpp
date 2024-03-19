// 
// sdp-programmer: CLI app for transferring data between a PC and sdp-firmware
//
// Copyright (c) 2024, Jonathan Tainer
// 

#include <stdio.h>
#include "../serialib/lib/serialib.h"

#define BAUDRATE 1152000

int main(int argc, char** argv) {

	// Check command line args
	if (argc < 2) {
		printf("No port specified\n");
		return 0;
	}
	if (argc < 3) {
		printf("No input file specified\n");
		return 0;
	}

	// Open serial port
	serialib serial;
	if (serial.openDevice(argv[1], BAUDRATE) != 1) {
		printf("Failed to open port: %s\n", argv[1]);
		return 1;
	}

	// Open input file
	FILE* input = fopen(argv[2], "r");
	if (input == NULL) {
		printf("Failed to open input file: %s\n", argv[2]);
		return 1;
	}

	// Get input file size
	fseek(input, 0, SEEK_END);
	long inputSize = ftell(input);
	rewind(input);

	// Clean up
	serial.closeDevice();
	fclose(input);

	return 0;
}

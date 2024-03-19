// 
// sdp-programmer: CLI app for transferring data between a PC and sdp-firmware
//
// Copyright (c) 2024, Jonathan Tainer
// 

#include <stdio.h>
#include "../serialib/lib/serialib.h"

#define BAUDRATE 1152000
#define MSG_DATA_LEN 256 // Max number of bytes encoded in each message
#define MSG_LEN (MSG_DATA_LEN * 2) + 3 // Each byte encoded as two hex characters, plus space for opcode, newline, and null characters
#define SECTOR_LEN 4096
#define FLASH_SIZE (16*1024*1024)

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
	if (inputSize > FLASH_SIZE) {
		printf("ERROR: Not enough space available\n");
		printf("\tInput file %s = %ld bytes\n", inputSize);
		printf("\tFlash size = %ld bytes\n", FLASH_SIZE);
		return 1;
	}

	int msgCount = inputSize / MSG_DATA_LEN;
	if (inputSize % MSG_DATA_LEN) msgCount++;
	int sectorCount = inputSize / SECTOR_LEN;
	if (inputSize % SECTOR_LEN) sectorCount++;
	printf("Using %ld of %ld available bytes\n", inputSize, FLASH_SIZE);
	printf("%d sectors to be erased\n", sectorCount);

	// Clean up
	serial.closeDevice();
	fclose(input);

	return 0;
}

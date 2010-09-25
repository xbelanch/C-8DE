#include "yace.h"

// Dumps a Memory image to stdout
void memory_dump(uint8_t* memory){
	
	int b;
	for (b = 0x000; b < 0x300; b++) {
		printf("%.2X ", memory[b]);
		if ((b % 4) == 3) {
			printf(" ");
		}
		if ((b % 16) == 15) {
			printf("\n");
		}
	}
	if ((b % 16) != 15) {
		printf("\n");
	}
	
}
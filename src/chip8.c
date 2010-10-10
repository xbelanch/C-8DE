#include "yace.h"


/*
FONTS
=====

Programs may also refer to a group of sprites representing the hexadecimal digits 0 through F. These sprites are 5 bytes long, or 8x5 pixels. The data should be stored in the interpreter area of Chip-8 memory (0x000 to 0x1FF)

For example:

"5"		Binary		Hex
------------------------
****	11110000	0xF0
*   	10000000	0x80
****	11110000	0xF0
   *	00010000	0x10
****	11110000	0xF0
*/


uint8_t fonts8x5[]={
	0xf0, 0x90, 0x90, 0x90, 0xf0,	/* 0 */
	0x20, 0x60, 0x20, 0x20, 0x70,	/* 1 */
	0xf0, 0x10, 0xf0, 0x80, 0xf0,	/* 2 */
	0xf0, 0x10, 0xf0, 0x10, 0xf0,	/* 3 */
	0x90, 0x90, 0xf0, 0x10, 0x10,	/* 4 */
	0xf0, 0x80, 0xf0, 0x10, 0xf0,	/* 5 */
	0xf0, 0x80, 0xf0, 0x90, 0xf0,	/* 6 */
	0xf0, 0x10, 0x20, 0x40, 0x40,	/* 7 */
	0xf0, 0x90, 0xf0, 0x90, 0xf0,	/* 8 */
	0xf0, 0x90, 0xf0, 0x10, 0xf0,	/* 9 */
	0xf0, 0x90, 0xf0, 0x90, 0x90,	/* A */
	0xe0, 0x90, 0xe0, 0x90, 0xe0,	/* B */
	0xf0, 0x80, 0x80, 0x80, 0xf0,	/* C */
	0xe0, 0x90, 0x90, 0x90, 0xe0,	/* D */
	0xf0, 0x80, 0xf0, 0x80, 0xf0,	/* E */
	0xf0, 0x80, 0xf0, 0x80, 0x80	/* F */
	
};





void reset_chip8(){
	int i;
	//Reset the whole machine 
	memset(&CHIP8, 0, sizeof(CHIP8));
	
	// Copy the CHIP-8 4x5 pixels fonts to CHIP-8 memory.
	memcpy(&CHIP8.memory[0x1b0], fonts8x5, sizeof(fonts8x5));
	
	// CHIP-8 programs start at address 0x200 
	CHIP8.pc = START_PROGRAM; 
	CHIP8.i  = START_PROGRAM;
	CHIP8.st = 0x0;
	CHIP8.dt = 0x0;
	//registers to 0
	for (i=0; i<=0xf; i++){
		CHIP8.v[i] = 0x0;
	}	
	
	//Clear the video buffer
	clear_videobuffer();
	
}

void load_game(const char* filename){
	FILE *file;
	
	file = fopen(filename, "rb");
	if (debug_flag)
		printf("Trying to open: %s \n", filename);
	if (!file){
		fprintf(stderr, "Unable to load: %s\n", filename);
		exit(-1);
	}
	fread(&CHIP8.memory[START_PROGRAM], sizeof(CHIP8.memory) - START_PROGRAM, 1, file);
	if (debug_flag)
		printf("OK!...\n");
	fclose(file);
}
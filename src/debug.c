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

void opcode_debug(uint16_t opcode, int i, int j){
	int display = 1;
	char text[40];
	
	switch(nibble4(opcode)){
		
		case 0x1:
			sprintf(text, "0x%.3X ... JP 0x%.3X            ", opcode, opcode & 0xfff);
			break;

		case 0x2:
			sprintf(text, "0x%.3X ... CALL 0x%.3X          ", opcode, opcode & 0xfff);
			break;
			
		case 0x3:
			sprintf(text, "0x%.3X ... SE v[%.1X], 0x%.2X     ", opcode, nibble3(opcode), opcode & 0xff);
			break;
		case 0x6:
			sprintf(text, "0x%.3X ... LD v[%.1X], 0x%.2X     ", opcode, nibble3(opcode), opcode & 0xff);
			break;
			
		case 0x7:
			sprintf(text, "0x%.3X ... ADD v[%.1X], 0x%.2X    ", opcode, nibble3(opcode), opcode & 0xff);
			break;
			
		case 0x8:
			switch(nibble1(opcode)){
					case 0x0:
						sprintf(text, "0x%.3X ... LD v[%.1X], v[%.1X]     ", opcode, nibble3(opcode), nibble2(opcode));
						break;
			}//end of switch 0x8
			break;
			
		case 0xA:
			sprintf(text, "0x%.3X ... LD I, 0x%.3X         ", opcode, opcode & 0xfff);
			break;

		case 0xC:
			sprintf(text, "0x%.3X ... RND V[%.1X], 0x%.2X    ", opcode, nibble3(opcode), opcode & 0xff);
			break;
		
		case 0xD:
			sprintf(text, "0x%.3X ... DRW V[%1.X], V[%1.X], %d ", opcode, nibble3(opcode), nibble2(opcode), nibble1(opcode));
			break;
		
		case 0xE:
			switch (opcode & 0xff){
				
				case 0xa1:
					sprintf(text, "0x%.3X ... SKNP V[%.1X]        ", opcode, nibble3(opcode));
					break;
			}//end of switch 0xe
			break;
		
		case 0xF:
			switch (opcode & 0xff){
				case 0x1e:
					sprintf(text, "0x%.3X ... ADD I, %d                ", opcode, nibble3(opcode));
					break;
				
				case 0x65:
					sprintf(text, "0x%.3X ... LD V[%.1X], I[0x%.3X]      ", opcode, nibble3(opcode), CHIP8.i, CHIP8.memory[CHIP8.i]);
					break;
				
				
			} //end of switch 0XF
			break;
			
		default:
			display = 0;
			break;			
	} //end	
	
	//stupid code
	if (display){
		//detecta si es el primer opcode...y lo pinta de amarillo!
		if (j==10) {
			drawYellowString(text, i, j, bcode, screen);
		} else {
			drawString(text, i, j, bcode, screen);
		}
	}
	
}

void chip8_debug(){
	char text[20];
	uint16_t opcode;
	
	opcode = CHIP8.memory[CHIP8.pc]<<8 | CHIP8.memory[CHIP8.pc+1];
	

	
	int i;
	int j;
	const char* registers[]={"V[0]","V[1]","V[2]","V[3]","V[4]", "V[5]","V[6]","V[7]","V[8]","V[9]","V[A]","V[B]","V[C]","V[D]","V[E]","V[F]"};


	sprintf(text, "PC: 0x%.3X", CHIP8.pc);
	drawYellowString(text, 10, 175, bcode, screen);

	sprintf(text, "Index: 0x%.3X", CHIP8.i);
	drawString(text, 10, 200, bcode, screen);
	sprintf(text, "SP: 0x%.3X", CHIP8.sp);
	drawString(text, 10, 225, bcode, screen);
	
	sprintf(text, "DT: %.1X  ST: %1.X", CHIP8.dt, CHIP8.st);
	drawString(text, 10, 250, bcode, screen);
	
	
	sprintf(text, "V[F]: %.2X", CHIP8.v[15]);
	drawString(text, 10, 275, bcode, screen);
	
	
	sprintf(text, "Opcode: 0x%.3X", opcode);
	drawYellowString(text, 150, 175, bcode, screen);
	//drawString("Registers", 440, 165,  code, screen);
	//drawString("---------", 440, 180,  code, screen);
	
	//draw opcodes
	for (i=0; i<7; i++){
		opcode = CHIP8.memory[CHIP8.pc+(i*2)]<<8 | CHIP8.memory[CHIP8.pc+(i*2)+1];
		opcode_debug(opcode, 340, 10 + (i*20));
	}

	//draw registers
	for (j=0; j<3; j++){
		for (i = 0; i<5; i++){
			sprintf(text, "%s=0x%.2X", registers[i + (j*5)], CHIP8.v[i+(j*5)]);
			drawString(text, 150 + (j*80), 200 + (i*18), code, screen);
		}
	}
	
	//draw Stack
	drawYellowString("STACK", 420, 175, bcode, screen);
	for (i=0; i<5; i++){
		sprintf(text, "0x%.3X", CHIP8.stack[i+1]);
		drawString(text, 420, 200 + (i*18), code, screen);
	}

	SDL_UpdateRect(screen, 0, 0, 0, 0);
}



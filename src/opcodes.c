/*
 *  opcodes.c - Yet Another CHIP-8 Emulator
 *  Copyright 2010 Xavier Belanche Alonso <xbelanch@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#include "yace.h"

//Standard Chip-8 Instructions
//Technical Reference by Cowgod's
//http://devernay.free.fr/hacks/chip8/C8TECH10.HTM
void fetch_opcode(){
	
	uint16_t opcode;
	
	opcode = CHIP8.memory[CHIP8.pc]<<8 | CHIP8.memory[CHIP8.pc+1];
	
	switch(nibble4(opcode)) {
		case 0x0:
			switch(opcode & 0xff){
				/*
				00e0 - CLS 
				Clear the display
				*/
				case 0xe0:
					clear_videobuffer();
					break;
				/*
				00ee - RET
				Return from a subroutine
				The interpreter sets the program counter to the address at the top of the stack, then subtracts 1 from the stack pointer.
				*/
				case 0xee:
					if (CHIP8.sp > 0){
						CHIP8.pc = CHIP8.stack[CHIP8.sp] - 2;
						CHIP8.sp--;
					}
					break;
			}
			break;
		
		/*
		1nnn - JP addr
		Jump to location nnn.
		The interpreter sets the program counter to nnn.
		*/	
		case 0x1:
			CHIP8.pc = (opcode & 0xfff) - 2;
			break;
		
		/*
		2nnn - CALL addr
		Call subroutine at nnn.
		The interpreter increments the stack pointer, then puts the current PC on the top of the stack. The PC is then set to nnn.
		*/
		case 0x2:
			if (CHIP8.sp < 16){
				CHIP8.sp++;
				CHIP8.stack[CHIP8.sp] = CHIP8.pc;
				CHIP8.pc = (opcode & 0xfff) - 2;
			}
			
			
		/*
		3xkk - SE Vx, byte
		Skip next instruction if Vx = kk.
		The interpreter compares register Vx to kk, and if they are equal, increments the program counter by 2.
		*/
		case 0x3:
			if (CHIP8.v[nibble3(opcode)] == (opcode & 0xff))
				CHIP8.pc +=2;
			break;
		
		/*
		4xkk - SNE Vx, byte
		Skip next instruction if Vx != kk.
		The interpreter compares register Vx to kk, and if they are not equal, increments the program counter by 2.
		*/
		case 0x4:
			if (CHIP8.v[nibble3(opcode)] != (opcode & 0xff))
				CHIP8.pc +=2;
			break;
			
		/*
		5xy0 - SE Vx, Vy
		Skip next instruction if Vx = Vy.

		The interpreter compares register Vx to register Vy, and if they are equal, increments the program counter by 2.
		*/
		case 0x5:
			if (CHIP8.v[nibble3(opcode)] == CHIP8.v[nibble2(opcode)])
				CHIP8.pc +=2;
			break;
		/* 
		6xkk - LD Vx, byte
		Set Vx = kk.
		The interpreter puts the value kk into register Vx.
		*/
		case 0x6:
			CHIP8.v[nibble3(opcode)] = (opcode & 0xff);
			break;
			
		/*	
		7xkk - ADD Vx, byte
		Set Vx = Vx + kk.
		Adds the value kk to the value of register Vx, then stores the result in Vx. 
		*/
		case 0x7:
			CHIP8.v[nibble3(opcode)] += (opcode & 0xff);
			break;
			
		//list of opcodes under 0x8	
		case 0x8:
			switch(nibble1(opcode)){
				/*
				8xy0 - LD Vx, Vy
				Set Vx = Vy
				Stores the value of register Vy in register Vx.
				*/
				case 0x0:
					CHIP8.v[nibble3(opcode)] = CHIP8.v[nibble2(opcode)];
					break;
					
				/*
				8xy1 - OR Vx, Vy
				Set Vx = Vx OR Vy.

				Performs a bitwise OR on the values of Vx and Vy, then stores the result in Vx. A bitwise OR compares the corrseponding bits from two values, and if either bit is 1, then the same bit in the result is also 1. Otherwise, it is 0.
				*/
				case 0x1:
					CHIP8.v[nibble3(opcode)] |= CHIP8.v[nibble2(opcode)];
					break;
				/*
				
				*/
				case 0x2:
					CHIP8.v[nibble3(opcode)] &= CHIP8.v[nibble2(opcode)];
					break;
				
				case 0x3:
					CHIP8.v[nibble3(opcode)] ^= CHIP8.v[nibble2(opcode)];
					break;
				/*
				8xy4 - ADD Vx, Vy
				Set Vx = Vx + Vy, set VF = carry.

				The values of Vx and Vy are added together. If the result is greater than 8 bits (i.e., > 255,) VF is set to 1, otherwise 0. Only the lowest 8 bits of the result are kept, and stored in Vx.
				*/
				case 0x4: {
					uint8_t i;
					
					CHIP8.v[15] = 0;
					
					i = CHIP8.v[nibble3(opcode)];
					CHIP8.v[nibble3(opcode)] += CHIP8.v[nibble2(opcode)];
					
					if (i > CHIP8.v[nibble3(opcode)])
						CHIP8.v[15] = 1;
					break;
				}
				/*
				8xy5 - SUB Vx, Vy
				Set Vx = Vx - Vy, set VF = NOT borrow.

				If Vx > Vy, then VF is set to 1, otherwise 0. Then Vy is subtracted from Vx, and the results stored in Vx.
				*/
				case 0x5:
					CHIP8.v[15] = 0;
					
					if (CHIP8.v[nibble3(opcode)] >= CHIP8.v[nibble2(opcode)])
						CHIP8.v[15] = 1;
					
					CHIP8.v[nibble3(opcode)] -= CHIP8.v[nibble2(opcode)];
					break;
					
				/*
				8xy6 - SHR Vx {, Vy}
				Set Vx = Vx SHR 1.
				If the least-significant bit of Vx is 1, then VF is set to 1, otherwise 0. Then Vx is divided by 2.
				*/
				case 0x6:
					CHIP8.v[16] = (CHIP8.v[nibble3(opcode)] & 0x1);
					CHIP8.v[nibble3(opcode)] >>= 1;
					break;
					
				/*
				8xy7 - SUBN Vx, Vy
				Set Vx = Vy - Vx, set VF = NOT borrow.
				If Vy > Vx, then VF is set to 1, otherwise 0. Then Vx is subtracted from Vy, and the results stored in Vx.
				*/
				case 0x7:
					CHIP8.v[15] = 0;
					
					if (CHIP8.v[nibble2(opcode)] >= CHIP8.v[nibble3(opcode)])
						CHIP8.v[15] = 1;
					
					CHIP8.v[nibble2(opcode)] -= CHIP8.v[nibble3(opcode)];
					break;
				
				/*
				8xyE - SHL Vx {, Vy}
				Set Vx = Vx SHL 1.

				If the most-significant bit of Vx is 1, then VF is set to 1, otherwise to 0. Then Vx is multiplied by 2.
				*/
				case 0xe:
					CHIP8.v[15] = ((CHIP8.v[nibble3(opcode)] & 0x80) ? 1 : 0);
					CHIP8.v[nibble3(opcode)] <<= 1;
					
				default:
					fprintf(stderr, "Invalid instruction: %04x\n", opcode);
			}
			
		/*
		9xy0 - SNE Vx, Vy
		Skip next instruction if Vx != Vy.
		The values of Vx and Vy are compared, and if they are not equal, the program counter is increased by 2.
		*/	
		case 0x9:
			if (CHIP8.v[nibble3(opcode)] != CHIP8.v[nibble2(opcode)])
				CHIP8.pc += 2;
			break;
			
		/*	
		Annn - LD I, addr
		Set I = nnn.
		The value of register I is set to nnn.	
		*/
		case 0xA:
			CHIP8.i = (opcode & 0xfff);
			break;	
		
		/*
		Bnnn - JP V0, addr
		Jump to location nnn + V0.
		The program counter is set to nnn plus the value of V0.
		*/
		case 0xB:
			CHIP8.pc = (opcode & 0xfff) + CHIP8.v[0] - 2;
			break;

		/*
		Cxkk - RND Vx, byte
		Set Vx = random byte AND kk.

		The interpreter generates a random number from 0 to 255, which is then ANDed with the value kk. The results are stored in Vx. See instruction 8xy2 for more information on AND.
		*/
		case 0xC:
			CHIP8.v[nibble3(opcode)] = rand8() & (opcode & 0xff);
			break;
			
		/* 
		Dxyn - DRW Vx, Vy, nibble
		Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.

		The interpreter reads n bytes from memory, starting at the address stored in I. These bytes are then displayed as sprites on screen at coordinates (Vx, Vy). Sprites are XORed onto the existing screen. If this causes any pixels to be erased, VF is set to 1, otherwise it is set to 0. If the sprite is positioned so part of it is outside the coordinates of the display, it wraps around to the opposite side of the screen.
		*/
		case 0xD:
			draw_pixels(opcode);
			break;		

		/*
		group of opcodes
		*/
		case 0xE:
			switch (opcode & 0xff){
				
				/*
				Ex9E - SKP Vx
				Skip next instruction if key with the value of Vx is pressed.

				Checks the keyboard, and if the key corresponding to the value of Vx is currently in the down position, PC is increased by 2.
				*/
				case 0x9e:
					if (CHIP8.keystate[CHIP8.v[nibble3(opcode)]])
						CHIP8.pc +=2;
					break;
				
				/*
				ExA1 - SKNP Vx
				Skip next instruction if key with the value of Vx is not pressed.

				Checks the keyboard, and if the key corresponding to the value of Vx is currently in the up position, PC is increased by 2.
				
				*/
				case 0xa1:
					if (!CHIP8.keystate[CHIP8.v[nibble3(opcode)]])
						CHIP8.pc +=2;
					break;
					
				default:
					fprintf(stderr, "Invalid instruction: %04x\n", opcode);
					break;
				}
					
		/*
		group of opcodes 0xFxxx
		*/
		case 0xF:
			switch (opcode & 0xff){
				/*
				Fx07 - LD Vx, DT
				Set Vx = delay timer value.

				The value of DT is placed into Vx.
				*/
				case 0x7:
					CHIP8.v[nibble3(opcode)] = CHIP8.dt;
					break;
				
				/*
				Fx0A - LD Vx, K
				Wait for a key press, store the value of the key in Vx.

				All execution stops until a key is pressed, then the value of that key is stored in Vx.
				
				*/
				case 0xa1:
					//to implement 
					//wait_keypress(opcode)
					wait_keypress(opcode);
					break;
					
				/*
				
				*/
				case 0x15:
					CHIP8.dt = CHIP8.v[nibble3(opcode)];
					break;
					
				/*
				
				*/
				case 0x18:
					CHIP8.st = CHIP8.v[nibble3(opcode)];
					break;
				
				/*
				Fx1E - ADD I, Vx
				Set I = I + Vx.

				The values of I and Vx are added, and the results are stored in I.
				*/
				case 0x1e:
					CHIP8.i += CHIP8.v[nibble3(opcode)];
					break;
				
				/*
				Fx29 - LD F, Vx
				Set I = location of sprite for digit Vx.

				The value of I is set to the location for the hexadecimal sprite corresponding to the value of Vx.
				*/
				case 0x29:
					CHIP8.i =  CHIP8.v[nibble3(opcode)] * 5;
					break;
				
				/*
				Fx33 - LD B, Vx
				Store BCD representation of Vx in memory locations I, I+1, and I+2.

				The interpreter takes the decimal value of Vx, and places the hundreds digit in memory at location in I, the tens digit at location I+1, and the ones digit at location I+2.
				*/
				case 0x33:
					CHIP8.memory[CHIP8.i] 	= CHIP8.v[nibble3(opcode)] / 100;
					CHIP8.memory[CHIP8.i+1] = (CHIP8.v[nibble3(opcode)] % 100) / 10;
					CHIP8.memory[CHIP8.i+2] = CHIP8.v[nibble3(opcode)] % 10;
					break;
					
				/*
				Fx55 - LD [I], Vx
				Store registers V0 through Vx in memory starting at location I.

				The interpreter copies the values of registers V0 through Vx into memory, starting at the address in I.
				*/
				case 0x55: {
					int i;
					
					for (i = 0; i <= nibble3(opcode); i++)
						CHIP8.memory[CHIP8.i+i] = CHIP8.v[i];
					break;
				}
				
				/*
				Fx65 - LD Vx, [I]
				Read registers V0 through Vx from memory starting at location I.

				The interpreter reads values from memory starting at location I into registers V0 through Vx.
				*/
				case 0x65: {
					int i;
					
					for (i = 0; i <= nibble3(opcode); i++)
						CHIP8.v[i] = CHIP8.memory[CHIP8.i+i];
						
					break;
				}
				
				default:
					fprintf(stderr, "Invalid instruction: %04x\n", opcode);
					break;
				
			}
			break;

		default:
			fprintf(stderr, "Fatal Invalid instruction: %04x\n", opcode);
			break;		
	}
	
	//Verificar el decremento
	if (CHIP8.dt)
		CHIP8.dt--;

	if (CHIP8.st)
		CHIP8.st--;
		
	//Program Counter (PC) +2
	CHIP8.pc +=2;
}		
		
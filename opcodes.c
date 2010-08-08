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


void fetch_opcode(){
	
	uint16_t opcode;
	
	opcode = CHIP8.memory[CHIP8.pc]<<8 | CHIP8.memory[CHIP8.pc+1];
	
	switch(nibble4(opcode)) {
		case 0x0:
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
		3xkk - SE Vx, byte
		Skip next instruction if Vx = kk.
		The interpreter compares register Vx to kk, and if they are equal, increments the program counter by 2.
		*/
		case 0x3:
			if (CHIP8.v[nibble3(opcode)] == (opcode & 0xff))
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
			
		/*	
		Annn - LD I, addr
		Set I = nnn.
		The value of register I is set to nnn.	
		*/
		case 0xA:
			CHIP8.i = (opcode & 0xfff);
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
	}

	//Program Counter (PC) +2
	CHIP8.pc +=2;
}		
		
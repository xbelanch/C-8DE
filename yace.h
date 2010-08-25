/*
 *  yace.h - Yet Another CHIP-8 Emulator
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


#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <SDL.h>
#include <SDL_ttf.h>


//Common variables
int debug_flag;
int pause_flag;
int step_flag;
Uint32 startTime, frameNumber;	
SDL_Surface *screen;

#define ZOOM 				5
#define FREQUENCY			60
#define FRAMERATE			60
#define FPS 				1000.f / FRAMERATE
#define WIN_WIDTH 			640
#define WIN_HEIGHT			320
#define CHIP8_SCREEN_WIDTH	64
#define	CHIP8_SCREEN_HEIGHT	32
#define BPP					32
#define PIXEL_W				1*ZOOM
#define PIXEL_H				1*ZOOM





#define LIBERATION_SERIF_REGULAR 	"fonts/LiberationSerif-Regular.ttf"
#define LIBERATION_SERIF_BOLD		"fonts/LiberationSerif-Bold.ttf"
#define LIBERATION_MONO_REGULAR		"fonts/LiberationMono-Regular.ttf"
#define LIBERATION_MONO_BOLD		"fonts/LiberationMono-Bold.ttf"

TTF_Font *header;
TTF_Font *text;
TTF_Font *code;
TTF_Font *bcode;

#define START_FIRMWARE		0x000
#define END_FIRMWARE		0x1ff
#define START_PROGRAM		0x200
#define END_PROGRAM			0x5ff
#define	STACK_ADDRESS		0x0e0	
#define MEMORY_SIZE 		0xfff
#define KEYBOARD_SIZE		16

//operations with bytes
#define addr(opcode)  				((opcode & 0x0fff))
#define nibble1(opcode) 			((opcode & 0xf))
#define nibble2(opcode) 			((opcode & 0xf0) >> 4)
#define nibble3(opcode) 			((opcode & 0xf00) >> 8)
#define nibble4(opcode) 			((opcode & 0xf000) >> 12)
#define low4bits_highbyte(opcode) 	((opcode & 0x0F00)>>8)
#define upper4bits_lowbyte(opcode)	((opcode & 0x00F0)>>4)

//misc
#define rand8()						0 + (int) (255 * (rand() / (RAND_MAX + 1.0)))




struct chip8 {
	// index register 
	uint16_t i;
	// program counter 
	uint16_t pc;
	// stack pointer 
	uint16_t sp;
	// Delay timer and sound timer
	uint8_t dt, st;
	// 16 purpose 8-bit register 
	uint8_t v[16];
	// stack
	uint16_t stack[16];
	// Video buffer
	uint8_t video_buffer[CHIP8_SCREEN_WIDTH][CHIP8_SCREEN_HEIGHT];
	// Memory of chip8
	uint8_t	memory[MEMORY_SIZE];
	// Keyboard handle
	uint8_t keystate[KEYBOARD_SIZE]	
} CHIP8;





//Chip8 methods
void reset_chip8();
void load_game(const char* filename);
void fetch_opcode();


//Debug methods
void memory_dump(uint8_t* memory);


//Video Methods
void video_init();
void clear_videobuffer();
void display_chip8_videobuffer();
void draw_pixels(uint16_t opcode);
void init_text_format();
void display_about();

//Timer
void time_sync();










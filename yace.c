/*
 *  yace.c - Yet Another CHIP-8 Emulator
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


void start_emulation(){

	
	//By default, debug is on
	debug_flag = 1;
	//Reset Chip-8
	reset_chip8();
	//Open the more simplistic game for Chip-8
	load_game("games/MAZE");
	//Dump the whole weird Memory to stdout
	memory_dump(CHIP8.memory);
	//Switch on the TV ;-)
	video_init();
	//Setup fonts
	init_text_format();

	//Clear the videobuffer
	clear_videobuffer();
	
	//display info
	display_about();
	//Enter the loop emulation!
	frameNumber = 0;
	for(;;){
		//executeCPU
		fetch_opcode();
		//generateInterrupts: keyboard, vertical retrace,..
		keyboard_chip8();	
		//emulateGraphics
		display_chip8_videobuffer();
		//emulateSound
		
		//timeSincronization
		time_sync();
		frameNumber++;
	}
	
}


int main (int argc, char *argv[]){
	// Set the seed for rand().
	srand(time(NULL));
	
	start_emulation();
	
	
}
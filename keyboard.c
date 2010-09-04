#include "yace.h"

chip8_key_t keymap[KEYBOARD_SIZE] = {
	CHIP8_KEY_0,
	CHIP8_KEY_1,
	CHIP8_KEY_2,
	CHIP8_KEY_3,
	CHIP8_KEY_4,
	CHIP8_KEY_5,
	CHIP8_KEY_6,
	CHIP8_KEY_7,
	CHIP8_KEY_8,
	CHIP8_KEY_9,
	CHIP8_KEY_a,
	CHIP8_KEY_b,
	CHIP8_KEY_c,
	CHIP8_KEY_d,
	CHIP8_KEY_e,
	CHIP8_KEY_f
};



void keyboard_chip8(){
	SDL_Event event;
	int i = 0;
	
	while(SDL_PollEvent(&event)){
		switch (event.type){
			case SDL_KEYDOWN:
				//special keys
				switch (event.key.keysym.sym){
					//exit the emulation
					case SDLK_ESCAPE:
						exit(1);
					
					case SDLK_SPACE:
						step_flag = 1;
						pause_flag = 0;
						break;
					
					case SDLK_p:
						pause_flag = !pause_flag;
						break;
				}
				//handle the 16 keys of Chip8
				for (i = 0; i < KEYBOARD_SIZE; i++){
					if (event.key.keysym.sym == (SDLKey)keymap[i]){
						CHIP8.keystate[i] = KEYSTATE_PRESSED;
						break;
					}
				}
				break;
			
			case SDL_KEYUP:
				for (i = 0; i < KEYBOARD_SIZE; i++){
					if (event.key.keysym.sym == (SDLKey)keymap[i]){
						CHIP8.keystate[i] = KEYSTATE_RELEASED;
						break;
					}
				}
			
				break;
			
			case SDL_QUIT:
				exit(1);
			
			default:
				break;
		}
	}
}


/* Wait indefinitely for the next available keyboard input event. */
void wait_keypress(uint16_t opcode){
	SDL_Event event;
	int i, flag = 0;

	while (SDL_WaitEvent(&event)) {
		switch (event.type) {
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE)
				exit(EXIT_SUCCESS);

			for (i = 0; i < KEYBOARD_SIZE; i++)
				if (event.key.keysym.sym == (SDLKey)keymap[i]) {
					CHIP8.v[nibble3(opcode)] = i;
					CHIP8.keystate[i] = KEYSTATE_PRESSED;
					flag = 1;
					break;
				}
			break;

		case SDL_QUIT:
			exit(EXIT_SUCCESS);

		default:
			break;
		}

		if (flag)
			break;
	}
}






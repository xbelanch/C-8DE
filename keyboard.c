#include "yace.h"


void keyboard_chip8(){
	SDL_Event event;
	
	while(SDL_PollEvent(&event)){
		switch (event.type){
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE)
					exit(1);
				break;
			
			
			case SDL_KEYUP:
				break;
			
			case SDL_QUIT:
				exit(1);
			
			default:
				break;
		}
	}
}
#include "yace.h"

//Explicar un poco mejor esto, no?
void time_sync(){
	char text[20];
	Uint32 tt, ts;
	float fps;
	
	ts = SDL_GetTicks();
	tt = startTime + frameNumber * FPS;
	fps = ( frameNumber/(float)(SDL_GetTicks() - startTime) )*1000;
	if (debug_flag){
		sprintf(text, "FPS: %.3f", fps);
		drawString(text, 10, 300, bcode,  screen);
	}
	if (ts <= tt){
		SDL_Delay(tt-ts);
	} else {
		frameNumber = 0;
		startTime = SDL_GetTicks();
	}

}

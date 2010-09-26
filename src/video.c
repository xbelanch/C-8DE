#include "yace.h"


//Initialize Video
void video_init()
{
	/* Initialize defaults, Video and Audio */
	if( SDL_Init(SDL_INIT_VIDEO)==(-1) ) { 
		printf("Could not initialize SDL: %s.\n", SDL_GetError());
		exit(-1);
	}
	
	screen = SDL_SetVideoMode( WIN_WIDTH, WIN_HEIGHT, BPP, SDL_SWSURFACE );
	
	if( screen == NULL ) {
		fprintf(stderr, "Couldn't set %ix%ix%i video mode: %s\n", WIN_WIDTH, WIN_HEIGHT, BPP, SDL_GetError());
		exit(-1);
	}
	
	SDL_ShowCursor( SDL_DISABLE ); // The cursor is ugly :)
	SDL_WM_SetCaption("Chip8 Didactic Emulator", NULL);
	
	/* Set up the SDL_TTF */
	TTF_Init();
	atexit(TTF_Quit);
	atexit(SDL_Quit);

}


//Handle the fonts
TTF_Font *loadFont(char *name, int size)
{
	/* Use SDL_TTF to load the font at the specified size */
	TTF_Font *font = TTF_OpenFont(name, size);
	if (font == NULL)
	{
		printf("Failed to open Font %s: %s\n", name, TTF_GetError());
		exit(1);
	}
	return font;
}


void closeFont(TTF_Font *font)
{
	/* Close the font once we're done with it */	
	if (font != NULL)
	{
		TTF_CloseFont(font);
	}
}


void drawString(char *text, int x, int y, TTF_Font *font, SDL_Surface *screen)
{
	SDL_Rect dest;
	SDL_Surface *surface;
	SDL_Color foregroundColor, backgroundColor;
	
	/* Green text on a black background */
	foregroundColor.r = 0xff;
	foregroundColor.g = 0xff;
	foregroundColor.b = 0xff;
	
	backgroundColor.r = 0;
	backgroundColor.g = 0;
	backgroundColor.b = 0;
	
	/* Use SDL_TTF to generate a string image, this returns an SDL_Surface */
	surface = TTF_RenderUTF8_Shaded(font, text, foregroundColor, backgroundColor);

	if (surface == NULL)
	{
		printf("Couldn't create String %s: %s\n", text, SDL_GetError());
		return;
	}
	
	/* Blit the entire surface to the screen */
	dest.x = x;
	dest.y = y;
	dest.w = surface->w;
	dest.h = surface->h;

	SDL_BlitSurface(surface, NULL, screen, &dest);
	/* Free the generated string image */
	SDL_FreeSurface(surface);
}

//stupid copy!
void drawYellowString(char *text, int x, int y, TTF_Font *font, SDL_Surface *screen)
{
	SDL_Rect dest;
	SDL_Surface *surface;
	SDL_Color foregroundColor, backgroundColor;
	
	/* Green text on a black background */
	foregroundColor.r = 0xff;
	foregroundColor.g = 0xff;
	foregroundColor.b = 0x00;
	
	backgroundColor.r = 0;
	backgroundColor.g = 0;
	backgroundColor.b = 0;
	
	/* Use SDL_TTF to generate a string image, this returns an SDL_Surface */
	surface = TTF_RenderUTF8_Shaded(font, text, foregroundColor, backgroundColor);

	if (surface == NULL)
	{
		printf("Couldn't create String %s: %s\n", text, SDL_GetError());
		return;
	}
	
	/* Blit the entire surface to the screen */
	dest.x = x;
	dest.y = y;
	dest.w = surface->w;
	dest.h = surface->h;

	SDL_BlitSurface(surface, NULL, screen, &dest);
	/* Free the generated string image */
	SDL_FreeSurface(surface);
}



void init_text_format()
{
	header 	= loadFont(LIBERATION_SERIF_BOLD, 16);
	text 	= loadFont(LIBERATION_SERIF_REGULAR, 14);
	code 	= loadFont(LIBERATION_MONO_REGULAR, 14);
	bcode	= loadFont(LIBERATION_MONO_BOLD, 16);
}


void display_about()
{
	drawString("YACE", 440, 0, header,  screen);
	drawString("Yet Another Chip8 Emulator", 440, 20, text,  screen);
	SDL_UpdateRect(screen, 0, 0, 0, 0);
}

void clear_videobuffer()
{
	int value, i, j;
	for (i=0; i<CHIP8_SCREEN_WIDTH; i++){
		for (j=0; j<CHIP8_SCREEN_HEIGHT; j++){
			CHIP8.video_buffer[i][j] = 0 ;
		}
	}
}



void display_chip8_videobuffer()
{
	int i,j;
	SDL_Rect rect = { 0, 0, PIXEL_W, PIXEL_H};
	
	//SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 0, 0, 0));
	
	for (i=0; i<CHIP8_SCREEN_WIDTH; i++){
		for (j=0; j<CHIP8_SCREEN_HEIGHT; j++){
			rect.x = i*PIXEL_W;
			rect.y = j*PIXEL_H;
			
			if(CHIP8.video_buffer[i][j]){
				SDL_FillRect(screen, &rect, SDL_MapRGB(screen->format, 0, 255, 0));	
			} else {
				SDL_FillRect(screen, &rect, SDL_MapRGB(screen->format, 155, 55, 100));
			}
		}
	}
	SDL_UpdateRect(screen, 0, 0, CHIP8_SCREEN_WIDTH*PIXEL_W, CHIP8_SCREEN_HEIGHT*PIXEL_H);
}


//Draw pixels onto video buffer of Chip8
void draw_pixels(uint16_t opcode)
{
	uint8_t pixel_value;
	int i, j, x, y;
	int line;
	//register 0xf = 0
	CHIP8.v[15] = 0;
	CHIP8.sp++;
	CHIP8.stack[CHIP8.sp] = CHIP8.i;

	x = CHIP8.v[nibble3(opcode)] % CHIP8_SCREEN_WIDTH;
	y = CHIP8.v[nibble2(opcode)] % CHIP8_SCREEN_HEIGHT;


	
	for (j=0; j < nibble1(opcode); j++){
		line = CHIP8.memory[(CHIP8.i)+j];
		for (i=0; i < 8; i++){
			if (line & (0x80 >> i)){
				pixel_value = CHIP8.video_buffer[x+i][y+j];
				if (pixel_value == 1){
					CHIP8.v[15] = 1;
				}
				//the xor drawing mode!!
				pixel_value ^= 1;
				CHIP8.video_buffer[x+i][y+j] = pixel_value;
			} 
		}
	}

	CHIP8.i = CHIP8.stack[CHIP8.sp];
	CHIP8.sp--; 
	//
	display_chip8_videobuffer();
}




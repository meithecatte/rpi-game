#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include <stdio.h>

#define ERROR_ON(cond,mesg) if(cond){fprintf(stderr,mesg " error: %s\n", SDL_GetError());exit(1);}

SDL_Surface * loadImage(char * name);

SDL_Window * gWindow;
SDL_Renderer * gRenderer;

int main(void){
	ERROR_ON(SDL_Init(SDL_INIT_VIDEO) != 0, "SDL_Init")
	atexit(SDL_Quit);

	gWindow = SDL_CreateWindow("rpi-game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

	ERROR_ON(gWindow == NULL, "SDL_CreateWindow")

	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	ERROR_ON(gRenderer == NULL, "SDL_CreateRenderer")

	return 0;
}

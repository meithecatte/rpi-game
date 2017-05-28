#include <stdlib.h>
#include <unistd.h>

#include "global.h"

SDL_Surface * loadImage(char * name);

SDL_Window * gWindow = NULL;
SDL_Renderer * gRenderer = NULL;

SDL_Texture * gTextureSplash = NULL;
SDL_Texture * gTextureMenuBackground = NULL;

Mix_Chunk * gSoundStartup = NULL;

render_func_t gRenderFunc = NULL;
render_state_t gRenderState;
u16 gJoypad;
u8 gExit;

int main(void){
	ERROR_ON_SDL(SDL_Init(SDL_INIT_VIDEO) != 0, "SDL_Init"); atexit(SDL_Quit);
	ERROR_ON_IMG(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG), "IMG_Init"); atexit(IMG_Quit);
	ERROR_ON_MIX(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 1, 2048) < 0, "Mix_OpenAudio"); atexit(Mix_Quit);
	atexit(cleanup);

	gWindow = SDL_CreateWindow("rpi-game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		320, 240, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	ERROR_ON_SDL(!gWindow, "SDL_CreateWindow");
	SDL_ShowCursor(SDL_DISABLE);

	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	ERROR_ON_SDL(!gRenderer, "SDL_CreateRenderer");

	SDL_SetRenderDrawColor(gRenderer, 100, 149, 237, 255);

	init_joypad();
	gRenderFunc = Render_SplashScreen;
	gRenderState.splash.frameCounter = 0;
	gExit = 0;

	gTextureSplash = loadTexture("assets/splash.png");
	gTextureMenuBackground = loadTexture("assets/menu_background.png");

	gSoundStartup = Mix_LoadWAV("assets/startup.wav");
	ERROR_ON_MIX(!gSoundStartup, "startup.wav load");

	while(!gExit){
		gJoypad = read_joypad();

		SDL_RenderClear(gRenderer);
		gRenderFunc();
		SDL_RenderPresent(gRenderer);
	}

	return 0;
}

void Render_SplashScreen(void){
	if(gRenderState.splash.frameCounter == 0){
		Mix_PlayChannel(-1, gSoundStartup, 0);
	}

	SDL_RenderCopy(gRenderer, gTextureSplash, NULL, NULL);

	if(gJoypad & JOY_START){
		gExit = 1;
	}

	gRenderState.splash.frameCounter++;
}

void cleanup(void){
	if(gTextureSplash)		SDL_DestroyTexture(gTextureSplash);
	if(gTextureMenuBackground)	SDL_DestroyTexture(gTextureMenuBackground);

	if(gSoundStartup)		Mix_FreeChunk(gSoundStartup);

	if(gRenderer)			SDL_DestroyRenderer(gRenderer);
	if(gWindow)			SDL_DestroyWindow(gWindow);
}

SDL_Texture * loadTexture(char * path){
	printf("Loading %s\n", path);

	SDL_Surface * surface = IMG_Load(path);
	ERROR_ON_IMG(surface == NULL, "IMG_Load");

	SDL_Texture * texture = SDL_CreateTextureFromSurface(gRenderer, surface);
	ERROR_ON_SDL(texture == NULL, "SDL_CreateTextureFromSurface");

	SDL_FreeSurface(surface);

	return texture;
}

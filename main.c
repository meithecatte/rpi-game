#include "global.h"
#include "joypad.h"
#include "main.h"
#include "ui.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

// power of two recommended for performance - untested
#define FRAME_VALUES 8

#define X(var,path,key) SDL_Texture * var = NULL;
IMAGE_LIST
#undef X

SDL_Window * gWindow = NULL;
SDL_Renderer * gRenderer = NULL;
SDL_Texture * gScreen = NULL;

void_func_t gRenderFunc = NULL;

bool gExit;
uint8_t gScreenFade = 255;
uint32_t gFrameTimes[FRAME_VALUES];
uint32_t gFrameTimeLast;

int main(void){
	Init_Joypad();
	Init_Random();
	Init_SDL();
	Init_FPS();
	Load_Resources();

	gRenderFunc = Render_SplashScreen;
	gExit = false;

	while(!gExit){
		SDL_SetRenderTarget(gRenderer, gScreen);

		Read_Joypad();
		gRenderFunc();

		SDL_SetRenderTarget(gRenderer, NULL);
		SDL_SetTextureColorMod(gScreen, gScreenFade,
			gScreenFade, gScreenFade);
		SDL_RenderCopy(gRenderer, gScreen, NULL, NULL);

		Measure_FPS();

		SDL_RenderPresent(gRenderer);
	}

	return 0;
}

void Load_Resources(void){
#define X(var,path,key) var = Load_Texture(path,key);
	IMAGE_LIST
#undef X

	GameSelectMenu_PrepareBackground();

	for(int i = 0;i < GAME_COUNT;i++){
		GameSelectMenu_RenderGame(&gGames[i]);
	}
}

void Dealloc_Resources(void){
	for(int i = 0;i < GAME_COUNT;i++){
		if(gGames[i].menuIcon){
			SDL_DestroyTexture(gGames[i].menuIcon);
		}
	}

#define X(var,path,key) if(var) SDL_DestroyTexture(var);
IMAGE_LIST
#undef X

	if(gScreen)   SDL_DestroyTexture(gScreen);
	if(gRenderer) SDL_DestroyRenderer(gRenderer);
	if(gWindow)   SDL_DestroyWindow(gWindow);
}

void Init_Random(void){
	uint32_t seed;
	int fd = open("/dev/hwrng", O_RDONLY);
	ERROR_ON_SYS(fd < 0, "open hwrng");
	ERROR_ON_SYS(read(fd, &seed, 4) != 4, "read hwrng");
	close(fd);
	srand(seed);
}

void Init_SDL(void){
	ERROR_ON_SDL(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0,
		"SDL_Init"); atexit(SDL_Quit);
	ERROR_ON_IMG(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG),
		"IMG_Init"); atexit(IMG_Quit);
	ERROR_ON_MIX(Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT,
		1, 1024) < 0, "Mix_OpenAudio"); atexit(Mix_Quit);
	atexit(Dealloc_Resources);

	gWindow = SDL_CreateWindow("rpi-game", SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT,
		SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	ERROR_ON_SDL(!gWindow, "SDL_CreateWindow");
	SDL_ShowCursor(SDL_DISABLE);

	gRenderer = SDL_CreateRenderer(gWindow, -1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	ERROR_ON_SDL(!gRenderer, "SDL_CreateRenderer");
	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);

	gScreen = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_ABGR8888,
		SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);
	ERROR_ON_SDL(!gScreen, "SDL_CreateTexture")
}

void Init_FPS(void){
	memset(gFrameTimes, 0, sizeof(gFrameTimes));
	gFrameTimeLast = SDL_GetTicks();
}

void Measure_FPS(void){
	static int frameCount;
	uint32_t currentTicks;
	float fps;
	
	SetFontColor16(0x32, 0xCD, 0x32);

	currentTicks = SDL_GetTicks();
	gFrameTimes[frameCount] = currentTicks - gFrameTimeLast;
	gFrameTimeLast = currentTicks;
	frameCount++;
	frameCount %= FRAME_VALUES;

	fps = 0;
	for(int i = 0;i < FRAME_VALUES;i++){
		fps += gFrameTimes[i];
	}

	fps /= FRAME_VALUES;
	fps = 1000.f / fps;

	char * str = NULL;
	asprintf(&str, "%d FPS; %d on heap",
		(int)(fps + 0.5f), gMallocCount);
	RenderText16(0, 0, str);
	free(str); // TEST_EXCEPTION - allocated by asprintf
}

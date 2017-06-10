#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "global.h"

// power of two recommended for performance - untested
#define FRAME_VALUES 8

#define X(var,path,key) SDL_Texture * var = NULL;
IMAGE_LIST
#undef X

SDL_Window * gWindow = NULL;
SDL_Renderer * gRenderer = NULL;
SDL_Texture * gScreen = NULL;

render_func_t gRenderFunc = NULL;

u8 gExit;
u8 gScreenFade = 255;
u32 gFrameTimes[FRAME_VALUES];
u32 gFrameTimeLast, gFrameCount;

int main(void){
	Init_Joypad();
	Init_Random();
	Init_SDL();
	Init_FPS();
	Load_Resources();

	gRenderFunc = Render_SplashScreen;
	gExit = 0;

	while(!gExit){
		SDL_SetRenderTarget(gRenderer, gScreen);

		Read_Joypad();
		gRenderFunc();

		SDL_SetRenderTarget(gRenderer, NULL);
		SDL_SetTextureColorMod(gScreen, gScreenFade, gScreenFade, gScreenFade);
		SDL_RenderCopy(gRenderer, gScreen, NULL, NULL);

		Measure_FPS();

		SDL_RenderPresent(gRenderer);
	}

	return 0;
}

void Load_Resources(void){
#define X(var,path,key) var = LoadTexture(path,key);
	IMAGE_LIST
#undef X

	for(u8 i = 0;i < GAME_COUNT;i++){
		gGames[i].menuIcon = LoadTexture(gGames[i].menuIconPath, KEY_NONE);
	}
}

void Dealloc_Resources(void){
	for(u8 i = 0;i < GAME_COUNT;i++){
		if(gGames[i].menuIcon) SDL_DestroyTexture(gGames[i].menuIcon);
	}

#define X(var,path,key) if(var) SDL_DestroyTexture(var);
IMAGE_LIST
#undef X

	if(gRenderer)	SDL_DestroyRenderer(gRenderer);
	if(gWindow)	SDL_DestroyWindow(gWindow);
}

void Init_Random(void){
	u32 seed;
	int fd = open("/dev/hwrng", O_RDONLY);
	ERROR_ON_SYS(fd < 0, "open hwrng");
	ERROR_ON_SYS(read(fd, &seed, 4) != 4, "read hwrng");
	close(fd);
	srand(seed);
}

void Init_SDL(void){
	ERROR_ON_SDL(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0, "SDL_Init"); atexit(SDL_Quit);
	ERROR_ON_IMG(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG), "IMG_Init"); atexit(IMG_Quit);
	ERROR_ON_MIX(Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 1, 1024) < 0, "Mix_OpenAudio"); atexit(Mix_Quit);
	atexit(Dealloc_Resources);

	gWindow = SDL_CreateWindow("rpi-game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	ERROR_ON_SDL(!gWindow, "SDL_CreateWindow");
	SDL_ShowCursor(SDL_DISABLE);

	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	ERROR_ON_SDL(!gRenderer, "SDL_CreateRenderer");
	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);

	gScreen = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_ABGR8888,
		SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void Init_FPS(void){
	memset(gFrameTimes, 0, sizeof(gFrameTimes));
	gFrameCount = 0;
	gFrameTimeLast = SDL_GetTicks();
}

void Measure_FPS(void){
	u32 mCurrentTicks, mDisplayFPS;
	float mFPS;
	
	SetFontColor16(0x32, 0xCD, 0x32);

	mCurrentTicks = SDL_GetTicks();
	gFrameTimes[gFrameCount % FRAME_VALUES] = mCurrentTicks - gFrameTimeLast;
	gFrameTimeLast = mCurrentTicks;
	gFrameCount++;

	mFPS = 0;
	for(u8 i = 0;i < FRAME_VALUES;i++){
		mFPS += gFrameTimes[i];
	}

	mFPS /= FRAME_VALUES;
	mFPS = 1000.f / mFPS;
	mDisplayFPS = floorf(mFPS + 0.5f);
	if(mDisplayFPS > 99) mDisplayFPS = 99;

	RenderChar16(0, 0, (mDisplayFPS / 10) + '0');
	RenderChar16(8, 0, (mDisplayFPS % 10) + '0');
}

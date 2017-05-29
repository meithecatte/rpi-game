#include <stdlib.h>
#include <unistd.h>

#include "global.h"

// power of two recommended for performance - untested
#define FRAME_VALUES 8

SDL_Window * gWindow = NULL;
SDL_Renderer * gRenderer = NULL;
SDL_Texture * gScreen = NULL;

#define X(var,path,key) SDL_Texture * var = NULL;
IMAGE_LIST
#undef X

render_func_t gRenderFunc = NULL;
render_func_t gRenderFuncAfterFade = NULL;
render_state_t gRenderState;
u16 gJoypad;
u8 gExit;
u8 gScreenFade = 255;

int main(void){
	ERROR_ON_SDL(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0, "SDL_Init"); atexit(SDL_Quit);
	ERROR_ON_IMG(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG), "IMG_Init"); atexit(IMG_Quit);
	ERROR_ON_MIX(Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 1, 1024) < 0, "Mix_OpenAudio"); atexit(Mix_Quit);
	atexit(cleanup);

	gWindow = SDL_CreateWindow("rpi-game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	ERROR_ON_SDL(!gWindow, "SDL_CreateWindow");
	SDL_ShowCursor(SDL_DISABLE);

	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	ERROR_ON_SDL(!gRenderer, "SDL_CreateRenderer");
	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);

	gScreen = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);

	init_joypad();
	gRenderFunc = Render_SplashScreen;
	gRenderState.splash.frameCounter = 0;
	gExit = 0;

#define X(var,path,key) var = loadTexture(path,key);
IMAGE_LIST
#undef X

	for(u8 i = 0;i < GAME_COUNT;i++){
		gGames[i].menuIcon = loadTexture(gGames[i].menuIconPath, KEY_NONE);
		if(gGames[i].initFunction) gGames[i].initFunction();
	}

	u32 mFrameTimes[FRAME_VALUES];
	u32 mFrameTimeLast, mFrameCount, mCurrentTicks, mDisplayFPS;
	float mFPS;
	
	memset(mFrameTimes, 0, sizeof(mFrameTimes));
	mFrameCount = 0;
	mFrameTimeLast = SDL_GetTicks();

	while(!gExit){
		gJoypad = read_joypad();

		SDL_SetRenderTarget(gRenderer, gScreen);
		SDL_RenderClear(gRenderer);
		gRenderFunc();
		SDL_SetRenderTarget(gRenderer, NULL);
		SDL_SetTextureColorMod(gScreen, gScreenFade, gScreenFade, gScreenFade);
		SDL_RenderCopy(gRenderer, gScreen, NULL, NULL);

		SetFontColor(0x32, 0xCD, 0x32);

		mCurrentTicks = SDL_GetTicks();
		mFrameTimes[mFrameCount % FRAME_VALUES] = mCurrentTicks - mFrameTimeLast;
		mFrameTimeLast = mCurrentTicks;
		mFrameCount++;

		mFPS = 0;
		for(u8 i = 0;i < FRAME_VALUES;i++){
			mFPS += mFrameTimes[i];
		}

		mFPS /= FRAME_VALUES;
		mFPS = 1000.f / mFPS;
		mDisplayFPS = floorf(mFPS + 0.5f);
		if(mDisplayFPS > 99) mDisplayFPS = 99;

		RenderChar(0, 0, (mDisplayFPS / 10) + '0');
		RenderChar(8, 0, (mDisplayFPS % 10) + '0');

		SDL_RenderPresent(gRenderer);

		if(gJoypad == (JOY_A | JOY_B | JOY_SELECT | JOY_START)) gExit = 1;
	}

	return 0;
}

void Render_SplashScreen(void){
	SDL_RenderCopy(gRenderer, gTextureSplash, NULL, NULL);

	gRenderState.splash.frameCounter++;

	if(gRenderState.splash.frameCounter >= 120){
		if(gScreenFade > FADE_SPEED) gScreenFade -= FADE_SPEED;
		else if(gScreenFade != 0) gScreenFade = 0;

		if(gScreenFade == 0){
			gRenderFunc = Render_FadeIn;
			gRenderFuncAfterFade = Render_GameSelectMenu;
			gRenderState.gameSelectMenu.currentGame = 0;
			gRenderState.gameSelectMenu.scrollOffset = 0;
		}
	}
}

void Render_GameSelectMenu(void){
	SDL_Rect dstrect = { .x = 0, .y = 0, .w = 40, .h = 20 };
	for(int y = 0;y < 12;y++){
		for(int x = 0;x < 8;x++){
			SDL_RenderCopy(gRenderer, gTextureMenuBackground, NULL, &dstrect);
			dstrect.x += 40;
		}

		dstrect.y += 20;
		dstrect.x = 0;
	}

	// XXX: Make this support more than one game when needed

	dstrect.x = 112;
	dstrect.y = 64;
	dstrect.w = 96;
	dstrect.h = 96;
	SDL_RenderCopy(gRenderer, gGames[0].menuIcon, NULL, &dstrect);

	SetFontColor(0, 0, 0);
	RenderText((SCREEN_WIDTH - strlen(gGames[0].menuNameTop) * 8) / 2, 164, gGames[0].menuNameTop);

	if(gScreenFade != 255) return;
}

void Render_FadeIn(void){
	if(gScreenFade < 255 - FADE_SPEED) gScreenFade += FADE_SPEED;
	else if(gScreenFade != 255) gScreenFade = 255;

	if(gScreenFade == 255){
		gRenderFunc = gRenderFuncAfterFade;
	}

	gRenderFuncAfterFade();
}


void cleanup(void){
	for(u8 i = 0;i < GAME_COUNT;i++){
		if(gGames[i].cleanupFunction) gGames[i].cleanupFunction();
		if(gGames[i].menuIcon) SDL_DestroyTexture(gGames[i].menuIcon);
	}

#define X(var,path,key) if(var) SDL_DestroyTexture(var);
IMAGE_LIST
#undef X

	if(gRenderer)			SDL_DestroyRenderer(gRenderer);
	if(gWindow)			SDL_DestroyWindow(gWindow);
}

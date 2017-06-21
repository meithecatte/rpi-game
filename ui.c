#include "global.h"
#include "joypad.h"
#include "ui.h"
#define GAME_LOGO_WIDTH 96
#define GAME_LOGO_HEIGHT 96
#define MENU_BACKGROUND_WIDTH 40
#define MENU_BACKGROUND_HEIGHT 20
#define MENU_GAME_WIDTH GAME_LOGO_WIDTH
#define MENU_GAME_HEIGHT (GAME_LOGO_HEIGHT + 2 * 16 + 4)

SDL_Texture * gTextureExtendedBackground = NULL;

void Render_SplashScreen(void){
	static int frameCounter;
	SDL_RenderCopy(gRenderer, gTextureSplash, NULL, NULL);

	frameCounter++;

	if(frameCounter >= 60){
		gRenderFunc = Render_FadeTransition;
		gRenderFuncAfterFade = Render_GameSelectMenu;
	}
}

void Render_GameSelectMenu(void){
	static game_index_t currentGame = GAME_EKANS;
	static int scroll = 0;
	static int scrolldir = 0;
	static int bgscroll = 0;

	SDL_Rect rect;
	rect.x = bgscroll;
	rect.y = 0;
	rect.w = SCREEN_WIDTH;
	rect.h = SCREEN_HEIGHT;
	SDL_RenderCopy(gRenderer, gTextureExtendedBackground,
		&rect, NULL);
	
	int baseX = (SCREEN_WIDTH - MENU_GAME_WIDTH * 2) / 2 + scroll;

	rect.x = baseX;
	rect.y = (SCREEN_HEIGHT - MENU_GAME_HEIGHT * 2) / 2;
	rect.w = MENU_GAME_WIDTH * 2;
	rect.h = MENU_GAME_HEIGHT * 2;
	SDL_RenderCopy(gRenderer,
		gGames[currentGame].menuRendered, NULL, &rect);

	if(currentGame > 0){
		rect.x = baseX - 288;
		SDL_RenderCopy(gRenderer,
			gGames[currentGame - 1].menuRendered, NULL, &rect);
	}

	if(currentGame < GAME_COUNT - 1){
		rect.x = baseX + 288;
		SDL_RenderCopy(gRenderer,
			gGames[currentGame + 1].menuRendered, NULL, &rect);
	}

	scroll += scrolldir * 18;
	bgscroll -= scrolldir * 3;

	if(bgscroll >= MENU_BACKGROUND_WIDTH){
		bgscroll -= MENU_BACKGROUND_WIDTH;
	}

	if(bgscroll < 0){
		bgscroll += MENU_BACKGROUND_WIDTH;
	}

	if(abs(scroll) >= 144){
		scroll *= -1;
		currentGame -= scrolldir;
	}

	if(scroll == 0){
		scroll = 0;
		scrolldir = 0;
	}

	if(!gScreenFade) return; // don't check input if mid-fade render
	if(scrolldir) return; // don't check input if scrolling

	if(gJoypadPressed & (JOY_START | JOY_A)){
		CALL_UNLESS_NULL(gGames[currentGame].startFunction);
		gRenderFunc = Render_FadeTransition;
		gRenderFuncAfterFade = gGames[currentGame].renderFunction;
	}else if((gJoypadPressed & (JOY_LEFT | JOY_L)) &&
			currentGame > 0){
		scrolldir = 1;
	}else if((gJoypadPressed & (JOY_RIGHT | JOY_R)) &&
			currentGame < GAME_COUNT - 1){
		scrolldir = -1;
	}
}

void GameSelectMenu_PrepareBackground(void){
	gTextureExtendedBackground = SDL_CreateTexture(gRenderer,
		SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_TARGET,
		SCREEN_WIDTH + MENU_BACKGROUND_WIDTH,
		SCREEN_HEIGHT + MENU_BACKGROUND_HEIGHT);
	ERROR_ON_SDL(!gTextureExtendedBackground, "bg texture");
	SDL_SetRenderTarget(gRenderer, gTextureExtendedBackground);
	SDL_Rect dstrect;
	dstrect.w = MENU_BACKGROUND_WIDTH;
	dstrect.h = MENU_BACKGROUND_HEIGHT;
	for(int y = 0;y <= SCREEN_HEIGHT;y += MENU_BACKGROUND_HEIGHT){
		for(int x = 0;x <= SCREEN_WIDTH;x += MENU_BACKGROUND_WIDTH){
			dstrect.x = x;
			dstrect.y = y;
			SDL_RenderCopy(gRenderer, gTextureMenuBackground,
				NULL, &dstrect);
		}
	}
}

void GameSelectMenu_RenderGame(game_t * game){
	game->menuIcon = Load_Texture(game->menuIconPath, KEY_NONE);
	game->menuRendered = SDL_CreateTexture(gRenderer,
		SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_TARGET,
		MENU_GAME_WIDTH, MENU_GAME_HEIGHT);
	ERROR_ON_SDL(!game->menuRendered, "menu game render");
	SDL_SetTextureBlendMode(game->menuRendered,
		SDL_BLENDMODE_BLEND);
	SDL_SetRenderTarget(gRenderer, game->menuRendered);
	SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 0);
	SDL_RenderClear(gRenderer);
	SDL_Rect dstrect;
	dstrect.w = GAME_LOGO_WIDTH;
	dstrect.h = GAME_LOGO_HEIGHT;
	dstrect.x = 0;
	dstrect.y = 0;
	SDL_RenderCopy(gRenderer, game->menuIcon, NULL, &dstrect);

	SetFontColor16(0, 0, 0);
	RenderText16(
		(MENU_GAME_WIDTH - strlen(game->menuNameTop) * 8) / 2,
		GAME_LOGO_HEIGHT + 4, game->menuNameTop);
	RenderText16(
		(MENU_GAME_WIDTH - strlen(game->menuNameBottom) * 8) / 2,
		GAME_LOGO_HEIGHT + 4 + 16, game->menuNameBottom);
}

void GameExit(void){
	gRenderFunc = Render_FadeTransition;
	gRenderFuncAfterFade = Render_GameSelectMenu;
}

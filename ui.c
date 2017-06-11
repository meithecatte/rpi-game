#include "global.h"
#define GAME_LOGO_WIDTH 192
#define GAME_LOGO_HEIGHT 192
#define MENU_SPACE_BELOW_LOGO 4
#define MENU_BACKGROUND_WIDTH 40
#define MENU_BACKGROUND_HEIGHT 20

void Render_SplashScreen(void){
	static int frameCounter;
	SDL_RenderCopy(gRenderer, gTextureSplash, NULL, NULL);

	frameCounter++;

	if(frameCounter >= 60){
		gRenderFunc = Render_FadeTransition;
		gRenderFuncAfterFade = Render_GameSelectMenu;
	}
}

void Render_GameSelectMenu_RenderGame(const game_t * game, int dx){
	int x = (SCREEN_WIDTH - GAME_LOGO_WIDTH) / 2 + dx;
	int y = (SCREEN_HEIGHT - GAME_LOGO_HEIGHT - 2 * 16 + MENU_SPACE_BELOW_LOGO) / 2;
	SDL_Rect dstrect = { .x = x, .y = y, .w = GAME_LOGO_WIDTH, .h = GAME_LOGO_HEIGHT };
	SDL_RenderCopy(gRenderer, game->menuIcon, NULL, &dstrect);

	SetFontColor16(0, 0, 0);
	RenderText16((SCREEN_WIDTH - strlen(game->menuNameTop) * 8) / 2 + dx,
		y + GAME_LOGO_HEIGHT + MENU_SPACE_BELOW_LOGO, game->menuNameTop);
	RenderText16((SCREEN_WIDTH - strlen(game->menuNameBottom) * 8) / 2 + dx,
		y + GAME_LOGO_HEIGHT + 16 + MENU_SPACE_BELOW_LOGO, game->menuNameBottom);
}

void Render_GameSelectMenu(void){
	static game_index_t currentGame = GAME_EKANS;
	SDL_Rect dstrect = { .x = 0, .y = 0, .w = MENU_BACKGROUND_WIDTH, .h = MENU_BACKGROUND_HEIGHT };
	for(int y = 0;y < (SCREEN_HEIGHT / MENU_BACKGROUND_HEIGHT);y++){
		for(int x = 0;x < (SCREEN_WIDTH / MENU_BACKGROUND_WIDTH);x++){
			SDL_RenderCopy(gRenderer, gTextureMenuBackground, NULL, &dstrect);
			dstrect.x += MENU_BACKGROUND_WIDTH;
		}

		dstrect.y += MENU_BACKGROUND_HEIGHT;
		dstrect.x = 0;
	}
	
	// XXX: Make this support more than one game when needed
	Render_GameSelectMenu_RenderGame(&gGames[0], 0);

	if(!gScreenFade) return; // don't check input if mid-fade render

	if(gJoypadPressed & (JOY_START | JOY_A)){
		CALL_UNLESS_NULL(gGames[currentGame].startFunction);
		gRenderFunc = Render_FadeTransition;
		gRenderFuncAfterFade = gGames[0].renderFunction;
	}
}

void GameExit(void){
	gRenderFunc = Render_FadeTransition;
	gRenderFuncAfterFade = Render_GameSelectMenu;
}

#include "global.h"

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
	SDL_Rect dstrect = { .x = 112 + dx, .y = 64, .w = 96, .h = 96 };
	SDL_RenderCopy(gRenderer, game->menuIcon, NULL, &dstrect);

	SetFontColor16(0, 0, 0);
	RenderText16((SCREEN_WIDTH - strlen(game->menuNameTop) * 8) / 2, 164 + dx, game->menuNameTop);
	RenderText16((SCREEN_WIDTH - strlen(game->menuNameBottom) * 8) / 2, 180 + dx, game->menuNameBottom);
}

void Render_GameSelectMenu(void){
	static game_index_t currentGame = GAME_EKANS;
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
	Render_GameSelectMenu_RenderGame(&gGames[0], 0);

	if(!gScreenFade) return; // don't check input if mid-fade render

	if(gJoypadPressed & (JOY_START | JOY_A)){
		CALL_UNLESS_NULL(gGames[currentGame].startFunction);
		gRenderFunc = Render_FadeTransition;
		gRenderFuncAfterFade = gGames[0].renderFunction;
	}
}

#include "global.h"
#include "ekans.h"

const ekans_direction_t gEkansOppositeDirection[] = {
	[UP] = DOWN,
	[DOWN] = UP,
	[LEFT] = RIGHT,
	[RIGHT] = LEFT,
};

const char * gEkansDifficultyStrings[] = {
	[SLOW] =	"Slow",
	[NORMAL] =	"Normal",
	[FAST] =	"Fast",
	[INSANE] =	"Insane",
};

void Ekans_StartFunction(void){
	gRenderState.ekans.score = 0;
	gRenderState.ekans.difficulty = NORMAL;
	gRenderState.ekans.state = MAIN_MENU;
	gRenderState.ekans.head = NULL;
	gRenderState.ekans.tail = NULL;
	gRenderState.ekans.menuCursorLocation = 0;
	SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
}

void Ekans_RenderFunction(void){
	SDL_RenderClear(gRenderer);
	SDL_Rect dstrect;
	switch(gRenderState.ekans.state){
		case MAIN_MENU:
			dstrect.x = 112; // draw logo
			dstrect.y = 0;
			dstrect.w = 96;
			dstrect.h = 96;
			SDL_RenderCopy(gRenderer, gGames[GAME_EKANS].menuIcon, NULL, &dstrect);

			SetFontColor8(0, 0, 0);
			RenderChar8(116, 100 + 12 * gRenderState.ekans.menuCursorLocation, 0x01);
			RenderText8(124, 100, "New Game");
			RenderText8(124, 112, "High Scores");
			RenderText8(124, 124, "Exit");

			if(!gScreenFade) return; // rendering for fade, ignore inputs

			if(gRenderState.ekans.menuCursorLocation > 0 && gJoypadPressed & JOY_UP){
				gRenderState.ekans.menuCursorLocation--;
			}else if(gRenderState.ekans.menuCursorLocation < 2 && gJoypadPressed & JOY_DOWN){
				gRenderState.ekans.menuCursorLocation++;
			}else if(gJoypadPressed & (JOY_START | JOY_A)){
				switch(gRenderState.ekans.menuCursorLocation){
					case 2:
						gRenderState.gameSelectMenu.scrollOffset = 0;
						gRenderState.gameSelectMenu.currentGame = GAME_EKANS;
						gRenderState.gameSelectMenu.fireUp = GAME_NONE;
						gRenderFunc = Render_FadeTransition;
						gRenderFuncAfterFade = Render_GameSelectMenu;
						break;
				}
			}

			break;
		default:
			break;
	}
}

void Ekans_StopFunction(void){
	;
}

#include "global.h"
#include "ekans.h"

const ekans_direction_t gEkansOppositeDirection[] = {
	[UP] = DOWN,
	[DOWN] = UP,
	[LEFT] = RIGHT,
	[RIGHT] = LEFT,
};

const char * gEkansDifficultyStrings[] = {
	[SLOW] =	"   Slow >",
	[NORMAL] =	"< Normal >",
	[FAST] =	" < Fast >",
	[INSANE] =	"< Insane",
};

void Ekans_StartFunction(void){
	gRenderState.ekans.score = 0;
	gRenderState.ekans.difficulty = NORMAL;
	gRenderState.ekans.state = MAIN_MENU;
	gRenderState.ekans.head = NULL;
	gRenderState.ekans.tail = NULL;
	SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
}

void Ekans_RenderFunction(void){
	SDL_Rect dstrect;
	switch(gRenderState.ekans.state){
		case MAIN_MENU:
			dstrect.x = 112; // draw logo
			dstrect.y = 0;
			dstrect.w = 96;
			dstrect.h = 96;
			SDL_RenderCopy(gRenderer, gGames[GAME_EKANS].menuIcon, NULL, &dstrect);

			SetFontColor8(0, 0, 0);
			RenderText8((320 - 64) / 2, 100, "New Game");
			RenderText8((320 - 80) / 2, 116, gEkansDifficultyStrings[gRenderState.ekans.difficulty]);
			RenderText8((320 - 88) / 2, 132, "High Scores");
			RenderText8((320 - 32) / 2, 148, "Exit");
			break;
		default:
			break;
	}
}

void Ekans_StopFunction(void){
	;
}

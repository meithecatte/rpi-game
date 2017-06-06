#include "global.h"
#include "ekans.h"

const ekans_direction_t gEkansOppositeDirection[] = {
	[UP] = DOWN,
	[DOWN] = UP,
	[LEFT] = RIGHT,
	[RIGHT] = LEFT,
};

const char * gEkansDifficultyStrings[] = {
	[SLOW] =	" Slow",
	[NORMAL] =	"Normal",
	[FAST] =	" Fast",
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

			if(gJoypadPressed & JOY_UP){
				gRenderState.ekans.menuCursorLocation--;

				if(gRenderState.ekans.menuCursorLocation < 0){
					gRenderState.ekans.menuCursorLocation += 3;
				}
			}else if(gJoypadPressed & JOY_DOWN){
				gRenderState.ekans.menuCursorLocation++;
				gRenderState.ekans.menuCursorLocation %= 3;
			}else if(gJoypadPressed & (JOY_START | JOY_A)){
				switch(gRenderState.ekans.menuCursorLocation){
					case 0:
						gRenderState.ekans.state = DIFFICULTY;
						break;
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
		case DIFFICULTY:
			dstrect.x = 112; // draw logo
			dstrect.y = 0;
			dstrect.w = 96;
			dstrect.h = 96;
			SDL_RenderCopy(gRenderer, gGames[GAME_EKANS].menuIcon, NULL, &dstrect);

			SetFontColor8(0, 0, 0);
			RenderChar8(112, 112 + 12 * gRenderState.ekans.difficulty, 0x06);
			RenderChar8(176, 112 + 12 * gRenderState.ekans.difficulty, 0x05);
			RenderText8(80, 92, "Difficulty Select");
			for(u8 i = 0;i < LENGTH(gEkansDifficultyStrings);i++){
				RenderText8(124, 112 + i * 12, gEkansDifficultyStrings[i]);
			}

			if(gJoypadPressed & JOY_UP){
				if(gRenderState.ekans.difficulty == 0){
					gRenderState.ekans.difficulty += 4;
				}

				gRenderState.ekans.difficulty--;
			}else if(gJoypadPressed & JOY_DOWN){
				gRenderState.ekans.difficulty++;
				gRenderState.ekans.difficulty %= 4;
			}else if(gJoypadPressed & JOY_A){
				gRenderState.ekans.state = PLAYING;
			}else if(gJoypadPressed & JOY_B){
				gRenderState.ekans.state = MAIN_MENU;
			}
		default:
			break;
	}
}

void Ekans_StopFunction(void){
	;
}

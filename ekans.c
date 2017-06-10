#include "global.h"
#include "ekans.h"

const ekans_direction_t gEkansOppositeDirection[] = {
	[UP] = DOWN,
	[DOWN] = UP,
	[LEFT] = RIGHT,
	[RIGHT] = LEFT,
};

const char * gEkansDifficultyStrings[] = {
	[SLOW]   = " Slow",
	[NORMAL] = "Normal",
	[FAST]   = " Fast",
	[INSANE] = "Insane",
};

ekans_state_t gEkansState;

void Ekans_Prepend(u16 x, u16 y){
	Ekans_Segment * new_segment = malloc(sizeof(Ekans_Segment));
	ERROR_ON_SYS(!new_segment, "alloc segment");
	new_segment->prev = NULL;
	new_segment->next = gEkansState.head;
	new_segment->x = x;
	new_segment->y = y;
	gEkansState.head->prev = new_segment;
	gEkansState.head = new_segment;
}

u8 Ekans_CheckForSegmentAt(u16 x, u16 y){
	Ekans_Segment * curr = gEkansState.head;

	while(curr){
		if(curr->x == x && curr->y == y){
			return 1;
		}

		curr = curr->next;
	}

	return 0;
}

void Ekans_RandomizeFruitLocation(void){
	do {
		gEkansState.fruitX = rand() % EKANS_PLAYFIELD_WIDTH;
		gEkansState.fruitY = rand() % EKANS_PLAYFIELD_HEIGHT;
	}while(Ekans_CheckForSegmentAt(gEkansState.fruitX, gEkansState.fruitY));
}

void Ekans_RenderPlayfield(void){
	SDL_Rect dstrect;
	dstrect.w = 16;
	dstrect.h = 16;

	SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
	dstrect.x = gEkansState.fruitX * 16;
	dstrect.y = gEkansState.fruitY * 16;
	SDL_RenderFillRect(gRenderer, &dstrect);

	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
	Ekans_Segment * curr = gEkansState.head;
	while(curr){
		dstrect.x = curr->x * 16;
		dstrect.y = curr->y * 16;
		SDL_RenderFillRect(gRenderer, &dstrect);
		curr = curr->next;
	}
}

void Ekans_StartFunction(void){
	gEkansState.difficulty = NORMAL;
	gEkansState.state = MAIN_MENU;
	gEkansState.head = NULL;
	gEkansState.tail = NULL;
	gEkansState.menuCursorLocation = 0;
}

void Ekans_RenderFunction(void){
	SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
	SDL_RenderClear(gRenderer);
	SDL_Rect dstrect;
	switch(gEkansState.state){
		case MAIN_MENU:
			dstrect.x = 112; // draw logo
			dstrect.y = 0;
			dstrect.w = 96;
			dstrect.h = 96;
			SDL_RenderCopy(gRenderer, gGames[GAME_EKANS].menuIcon, NULL, &dstrect);

			SetFontColor8(0, 0, 0);
			RenderChar8(116, 100 + 12 * gEkansState.menuCursorLocation, 0x01);
			RenderText8(124, 100, "New Game");
			RenderText8(124, 112, "High Scores");
			RenderText8(124, 124, "Exit");

			if(!gScreenFade) return; // don't check input if mid-fade render

			if(gJoypadPressed & JOY_UP){
				gEkansState.menuCursorLocation--;

				if(gEkansState.menuCursorLocation < 0){
					gEkansState.menuCursorLocation += 3;
				}
			}else if(gJoypadPressed & JOY_DOWN){
				gEkansState.menuCursorLocation++;
				gEkansState.menuCursorLocation %= 3;
			}else if(gJoypadPressed & (JOY_START | JOY_A)){
				switch(gEkansState.menuCursorLocation){
					case 0:
						gEkansState.state = DIFFICULTY;
						break;
					case 2:
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
			RenderChar8(112, 112 + 12 * gEkansState.difficulty, 0x06);
			RenderChar8(176, 112 + 12 * gEkansState.difficulty, 0x05);
			RenderText8(80, 92, "Difficulty Select");
			for(u8 i = 0;i < LENGTH(gEkansDifficultyStrings);i++){
				RenderText8(124, 112 + i * 12, gEkansDifficultyStrings[i]);
			}

			if(gJoypadPressed & JOY_UP){
				if(gEkansState.difficulty == 0){
					gEkansState.difficulty += 4;
				}

				gEkansState.difficulty--;
			}else if(gJoypadPressed & JOY_DOWN){
				gEkansState.difficulty++;
				gEkansState.difficulty %= 4;
			}else if(gJoypadPressed & JOY_A){
				gEkansState.state = PLAYING;
				gEkansState.direction = LEFT;
				gEkansState.score = 0;
				gEkansState.framesPerUpdate = 2 * (4 - gEkansState.difficulty);
				gEkansState.framesSinceLastUpdate = 0;
				gEkansState.inputForNextUpdate = NONE;

				gEkansState.head = malloc(sizeof(Ekans_Segment));
				ERROR_ON_SYS(!gEkansState.head, "alloc segment");
				gEkansState.head->x = 10;
				gEkansState.head->y = 5;
				gEkansState.head->prev = NULL;
				gEkansState.head->next = NULL;

				Ekans_Prepend(9, 5);
				Ekans_Prepend(8, 5);
				Ekans_Prepend(7, 5);
				Ekans_Prepend(6, 5);
				Ekans_RandomizeFruitLocation();
			}else if(gJoypadPressed & JOY_B){
				gEkansState.state = MAIN_MENU;
			}

			break;
		case PLAYING:
			if(gEkansState.inputForNextUpdate == NONE){
				switch(gEkansState.direction){
					case UP:
					case DOWN:
						if(gJoypadHeld & JOY_LEFT){
							gEkansState.inputForNextUpdate = LEFT;
						}else if(gJoypadHeld & JOY_RIGHT){
							gEkansState.inputForNextUpdate = RIGHT;
						}

						break;
					case LEFT:
					case RIGHT:
						if(gJoypadHeld & JOY_UP){
							gEkansState.inputForNextUpdate = UP;
						}else if(gJoypadHeld & JOY_DOWN){
							gEkansState.inputForNextUpdate = DOWN;
						}
						
						break;
					case NONE:
						fprintf(stderr, "%s:%d: no direction WTF", __FILE__, __LINE__);
						exit(1);
				}
			}

			if(gEkansState.framesSinceLastUpdate >= gEkansState.framesPerUpdate){
				gEkansState.framesSinceLastUpdate = 0;
			}else{
				gEkansState.framesSinceLastUpdate++;
			}

			Ekans_RenderPlayfield();
			break;
		default:
			break;
	}
}

void Ekans_StopFunction(void){
	;
}

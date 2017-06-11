#include "global.h"
#include "helper.h"
#include "joypad.h"
#include "ekans.h"
#include <stdbool.h>
#include <string.h>

ekans_state_t gEkansState;
const int gEkansFramesPerUpdate[] = {
	[SLOW]   = 6,
	[NORMAL] = 4,
	[FAST]   = 3,
	[INSANE] = 2,
};

void Ekans_StartFunc(void){
	gEkansState.difficulty = NORMAL;
	gEkansState.state = MAIN_MENU;
	gEkansState.head = NULL;
	gEkansState.tail = NULL;
	gEkansState.menuCursorLocation = 0;

	for(int i = 0;i < LENGTH(gEkansState.highScores);i++){
		strcpy(gEkansState.highScores[i].name, "");
		gEkansState.highScores[i].score = 0;
	}
}

void Ekans_RenderFunc(void){
	SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
	SDL_RenderClear(gRenderer);

	switch(gEkansState.state){
	case MAIN_MENU:
		Ekans_MainMenu();
		break;
	case DIFFICULTY:
		Ekans_DifficultyMenu();
		break;
	case PLAYING:
		Ekans_ProcessInput();
		Ekans_LogicUpdate();
		Ekans_RenderPlayfield();
		break;
	case GAME_OVER:
		gEkansState.state = MAIN_MENU;
	default:
		break;
	}
}

void Ekans_StartGame(void){
	gEkansState.state = PLAYING;
	gEkansState.direction = LEFT;
	gEkansState.score = 0;
	gEkansState.framesSinceLastUpdate = 0;
	gEkansState.inputForNextUpdate = NONE;

	gEkansState.head = _malloc(sizeof(Ekans_Segment));
	gEkansState.head->x = 10;
	gEkansState.head->y = 5;
	gEkansState.head->prev = NULL;
	gEkansState.head->next = NULL;
	gEkansState.tail = gEkansState.head;

	Ekans_AddSegment(9, 5);
	Ekans_AddSegment(8, 5);
	Ekans_AddSegment(7, 5);
	Ekans_AddSegment(6, 5);
	Ekans_RandomizeFruitLocation();
}

void Ekans_RenderPlayfield(void){
	SDL_Rect dstrect;
	dstrect.w = 16;
	dstrect.h = 16;

	SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
	dstrect.x = gEkansState.fruitX * 16;
	dstrect.y = gEkansState.fruitY * 16;
	SDL_RenderFillRect(gRenderer, &dstrect);

	SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
	Ekans_Segment * curr = gEkansState.head;
	while(curr){
		dstrect.x = curr->x * 16;
		dstrect.y = curr->y * 16;
		SDL_RenderFillRect(gRenderer, &dstrect);
		curr = curr->next;
		SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
	}

	dstrect.x = 0;
	dstrect.y = 224;
	dstrect.w = 320;
	dstrect.h = 1;
	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
	SDL_RenderFillRect(gRenderer, &dstrect);
}

void Ekans_LogicUpdate(void){
	if(gEkansState.framesSinceLastUpdate >= gEkansFramesPerUpdate[gEkansState.difficulty]){
		gEkansState.framesSinceLastUpdate = 0;

		if(gEkansState.inputForNextUpdate != NONE) gEkansState.direction = gEkansState.inputForNextUpdate;
		gEkansState.inputForNextUpdate = NONE;

		int newHeadX = gEkansState.head->x;
		int newHeadY = gEkansState.head->y;

		switch(gEkansState.direction){
		case UP:	newHeadY--; break;
		case DOWN:	newHeadY++; break;
		case LEFT:	newHeadX--; break;
		case RIGHT:	newHeadX++; break;
		case NONE:	fprintf(stderr, "%s:%d: no direction WTF", __FILE__, __LINE__); exit(1);
		}

		if(newHeadX == gEkansState.fruitX && newHeadY == gEkansState.fruitY){
			Ekans_AddSegment(newHeadX, newHeadY);
			Ekans_RandomizeFruitLocation();
		}else{
			if(Ekans_IsOccupied(newHeadX, newHeadY)){
				// XXX: Check for highscore
				gEkansState.state = GAME_OVER;
			}

			Ekans_Segment * newHead = gEkansState.tail;
			newHead->x = newHeadX;
			newHead->y = newHeadY;

			gEkansState.head->prev = newHead;
			gEkansState.tail = newHead->prev;
			newHead->prev->next = NULL;
			newHead->prev = NULL;
			newHead->next = gEkansState.head;
			gEkansState.head = newHead;
		}
	}else{
		gEkansState.framesSinceLastUpdate++;
	}
}

void Ekans_ProcessInput(void){
	if(gEkansState.inputForNextUpdate == NONE){
		switch(gEkansState.direction){
		case UP:
		case DOWN:
			if(gJoypadPressed & JOY_LEFT){
				gEkansState.inputForNextUpdate = LEFT;
			}else if(gJoypadPressed & JOY_RIGHT){
				gEkansState.inputForNextUpdate = RIGHT;
			}

			break;
		case LEFT:
		case RIGHT:
			if(gJoypadPressed & JOY_UP){
				gEkansState.inputForNextUpdate = UP;
			}else if(gJoypadPressed & JOY_DOWN){
				gEkansState.inputForNextUpdate = DOWN;
			}
			
			break;
		case NONE:
			fprintf(stderr, "%s:%d: no direction WTF", __FILE__, __LINE__);
			exit(1);
		}
	}
}

void Ekans_AddSegment(int x, int y){
	Ekans_Segment * new_segment = _malloc(sizeof(Ekans_Segment));
	new_segment->prev = NULL;
	new_segment->next = gEkansState.head;
	new_segment->x = x;
	new_segment->y = y;
	gEkansState.head->prev = new_segment;
	gEkansState.head = new_segment;
}

bool Ekans_IsOccupied(int x, int y){
	Ekans_Segment * curr = gEkansState.head;

	if(x < 0 || x > EKANS_PLAYFIELD_WIDTH) return true;
	if(y < 0 || y > EKANS_PLAYFIELD_HEIGHT) return true;

	while(curr){
		if(curr->x == x && curr->y == y){
			return true;
		}

		curr = curr->next;
	}

	return false;
}

void Ekans_RandomizeFruitLocation(void){
	do {
		gEkansState.fruitX = rand() % EKANS_PLAYFIELD_WIDTH;
		gEkansState.fruitY = rand() % EKANS_PLAYFIELD_HEIGHT;
	}while(Ekans_IsOccupied(gEkansState.fruitX, gEkansState.fruitY));
}

#include "global.h"
#include "helper.h"
#include "joypad.h"
#include "ekans.h"
#include <string.h>

const int gEkansFramesPerUpdate[] = {
	[SLOW]   = 6,
	[NORMAL] = 4,
	[FAST]   = 3,
	[INSANE] = 2,
};

void Ekans_StartFunc(void){
	gEkansDifficulty = NORMAL;
	gEkansState = MAIN_MENU;
	gEkansHead = NULL;
	gEkansTail = NULL;
	gEkansMenuCursorLocation = 0;

	for(int i = 0;i < LENGTH(gEkansHighScores);i++){
		strcpy(gEkansHighScores[i].name, "");
		gEkansHighScores[i].score = 0;
	}
}

void Ekans_RenderFunc(void){
	SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
	SDL_RenderClear(gRenderer);

	switch(gEkansState){
	case MAIN_MENU:
		Ekans_MainMenu();
		break;
	case DIFFICULTY:
		Ekans_DifficultyMenu();
		break;
	case PLAYING:
		Ekans_ProcessInput();

		if(gEkansFramesSinceLastUpdate
				>= gEkansFramesPerUpdate[gEkansDifficulty]){
			gEkansFramesSinceLastUpdate = 0;
			Ekans_LogicUpdate();
		}else{
			gEkansFramesSinceLastUpdate++;
		}

		Ekans_RenderPlayfield();
		break;
	case GAME_OVER:
		gEkansState = MAIN_MENU;
	default:
		break;
	}
}

void Ekans_StartGame(void){
	gEkansState = PLAYING;
	gEkansDirection = LEFT;
	gEkansScore = 0;
	gEkansFramesSinceLastUpdate = 0;
	gEkansInputForNextUpdate = NONE;

	gEkansHead = _malloc(sizeof(Ekans_Segment));
	gEkansHead->x = 18;
	gEkansHead->y = 14;
	gEkansHead->prev = NULL;
	gEkansHead->next = NULL;
	gEkansTail = gEkansHead;

	Ekans_AddSegment(17, 14);
	Ekans_AddSegment(16, 14);
	Ekans_AddSegment(15, 14);
	Ekans_RandomizeFruitLocation();
}

void Ekans_LogicUpdate(void){
	if(gEkansInputForNextUpdate != NONE){
		gEkansDirection = gEkansInputForNextUpdate;
	}

	gEkansInputForNextUpdate = NONE;

	int newHeadX = gEkansHead->x;
	int newHeadY = gEkansHead->y;

	switch(gEkansDirection){
	case UP:	newHeadY--; break;
	case DOWN:	newHeadY++; break;
	case LEFT:	newHeadX--; break;
	case RIGHT:	newHeadX++; break;
	case NONE:	fprintf(stderr, "no direction WTF"); exit(1);
	}

	if(newHeadX < 0){
		newHeadX += EKANS_PLAYFIELD_WIDTH;
	}else if(newHeadX >= EKANS_PLAYFIELD_WIDTH){
		newHeadX -= EKANS_PLAYFIELD_WIDTH;
	}

	if(newHeadY < 0){
		newHeadY += EKANS_PLAYFIELD_HEIGHT;
	}else if(newHeadY >= EKANS_PLAYFIELD_HEIGHT){
		newHeadY -= EKANS_PLAYFIELD_HEIGHT;
	}

	if(newHeadX == gEkansFruitX &&
			newHeadY == gEkansFruitY){
		Ekans_AddSegment(newHeadX, newHeadY);
		Ekans_RandomizeFruitLocation();
	}else{
		if(Ekans_IsOccupied(newHeadX, newHeadY)){
			// XXX: Check for highscore
			gEkansState = GAME_OVER;
		}

		Ekans_Segment * newHead = gEkansTail;
		newHead->x = newHeadX;
		newHead->y = newHeadY;

		gEkansHead->prev = newHead;
		gEkansTail = newHead->prev;
		newHead->prev->next = NULL;
		newHead->prev = NULL;
		newHead->next = gEkansHead;
		gEkansHead = newHead;
	}
}

void Ekans_ProcessInput(void){
	if(gEkansInputForNextUpdate == NONE){
		switch(gEkansDirection){
		case UP:
		case DOWN:
			if(gJoypadPressed & JOY_LEFT){
				gEkansInputForNextUpdate = LEFT;
			}else if(gJoypadPressed & JOY_RIGHT){
				gEkansInputForNextUpdate = RIGHT;
			}

			break;
		case LEFT:
		case RIGHT:
			if(gJoypadPressed & JOY_UP){
				gEkansInputForNextUpdate = UP;
			}else if(gJoypadPressed & JOY_DOWN){
				gEkansInputForNextUpdate = DOWN;
			}
			
			break;
		case NONE:
			fprintf(stderr, "no direction WTF");
			exit(1);
		}
	}
}

void Ekans_AddSegment(int x, int y){
	Ekans_Segment * newSegment = _malloc(sizeof(Ekans_Segment));
	newSegment->prev = NULL;
	newSegment->next = gEkansHead;
	newSegment->x = x;
	newSegment->y = y;
	gEkansHead->prev = newSegment;
	gEkansHead = newSegment;
}

bool Ekans_IsOccupied(int x, int y){
	Ekans_Segment * curr = gEkansHead;

	if(Ekans_IsWall(x, y)) return true;

	while(curr){
		if(curr->x == x && curr->y == y){
			return true;
		}

		curr = curr->next;
	}

	return false;
}

bool Ekans_IsWall(int x, int y){
	if(x == 0 || x == EKANS_PLAYFIELD_WIDTH - 1){
		return y <= 5 || y >= EKANS_PLAYFIELD_HEIGHT - 6;
	}else if(y == 0 || y == EKANS_PLAYFIELD_HEIGHT - 1){
		return x <= 5 || x >= EKANS_PLAYFIELD_WIDTH - 6;
	}
	
	return false;
}

void Ekans_RandomizeFruitLocation(void){
	do {
		gEkansFruitX = rand() % EKANS_PLAYFIELD_WIDTH;
		gEkansFruitY = rand() % EKANS_PLAYFIELD_HEIGHT;
	}while(Ekans_IsOccupied(gEkansFruitX, gEkansFruitY));
}

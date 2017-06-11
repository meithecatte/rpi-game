#include "global.h"
#include "ekans.h"

#include <string.h>

ekans_state_t gEkansState;
const u8 gEkansFramesPerUpdate[] = {
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

	for(u8 i = 0;i < LENGTH(gEkansState.highScores);i++){
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

		u16 newHeadX = gEkansState.head->x;
		u16 newHeadY = gEkansState.head->y;

		switch(gEkansState.direction){
		case UP:	if(newHeadY > 0) newHeadY--; break;
		case DOWN:	if(newHeadY < EKANS_PLAYFIELD_HEIGHT - 1) newHeadY++; break;
		case LEFT:	if(newHeadX > 0) newHeadX--; break;
		case RIGHT:	if(newHeadX < EKANS_PLAYFIELD_WIDTH - 1) newHeadX++; break;
		case NONE:	fprintf(stderr, "%s:%d: no direction WTF", __FILE__, __LINE__); exit(1);
		}

		if(Ekans_CheckForSegmentAt(newHeadX, newHeadY)){
			// XXX: Check for highscore
			gEkansState.state = GAME_OVER;
		}else if(newHeadX == gEkansState.fruitX && newHeadY == gEkansState.fruitY){
			Ekans_AddSegment(newHeadX, newHeadY);
			Ekans_RandomizeFruitLocation();
		}else{
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
		u8 buttonsPressed = 0;
		if(gJoypadHeld & JOY_UP) buttonsPressed++;
		if(gJoypadHeld & JOY_DOWN) buttonsPressed++;
		if(gJoypadHeld & JOY_LEFT) buttonsPressed++;
		if(gJoypadHeld & JOY_RIGHT) buttonsPressed++;
		if(buttonsPressed == 1){
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
	}
}

void Ekans_AddSegment(u16 x, u16 y){
	Ekans_Segment * new_segment = _malloc(sizeof(Ekans_Segment));
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

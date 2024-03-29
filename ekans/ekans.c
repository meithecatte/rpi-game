#include "ekans.h"
#include "joypad.h"
#include <string.h>

const int gEkansFramesPerUpdate[] = {
	[SLOW]   = 6,
	[NORMAL] = 4,
	[FAST]   = 3,
	[INSANE] = 2,
};

const int gEkansScoreMultiplier[] = {
	[SLOW]   = 2,
	[NORMAL] = 3,
	[FAST]   = 4,
	[INSANE] = 5,
};

ekans_direction_t gEkansInputForNextUpdate;
ekans_direction_t gEkansDirection;
ekans_difficulty_t gEkansDifficulty;
ekans_state_t gEkansState;

Mix_Music* gEkansIngameMusic;
Mix_Music* gEkansTitlescreenMusic;

int gEkansLength, gEkansScore, gEkansVScore, gEkansVHighScore;
int gEkansFruitX, gEkansFruitY, gEkansMenuCursorLocation;
int gEkansFramesSinceLastUpdate;

Ekans_Segment * gEkansHead;
Ekans_Segment * gEkansTail;
Ekans_ScoresTableEntry gEkansHighscores[EKANS_NUM_SCORES];

void Ekans_StartFunc(void){
	gEkansIngameMusic = Load_Music("assets/ekans/ingame.wav");
	gEkansTitlescreenMusic =
		Load_Music("assets/ekans/titlescreen.wav");
	for(int i = 0;i < EKANS_NUM_SCORES;i++){
		strcpy(gEkansHighscores[i].name, "");
		gEkansHighscores[i].score = 0;
	}

	strcpy(gEkansScoreName, "A");

	Ekans_LoadHighscores();

	gEkansDifficulty = NORMAL;
	gEkansState = MAIN_MENU;
	gEkansHead = NULL;
	gEkansTail = NULL;
	gEkansMenuCursorLocation = 0;
	gEkansVHighScore = gEkansHighscores[0].score;
	gEkansTempTexture = SDL_CreateTexture(gRenderer,
		SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_TARGET,
		SCREEN_WIDTH, SCREEN_HEIGHT);
	ERROR_ON_SDL(!gEkansTempTexture, "SDL_CreateTexture")

	Mix_PlayMusic(gEkansTitlescreenMusic, -1);
}

void Ekans_RenderFunc(void){
	switch(gEkansState){
	case MAIN_MENU:
		SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
		SDL_RenderClear(gRenderer);

		Ekans_MainMenu();
		break;
	case DIFFICULTY:
		SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
		SDL_RenderClear(gRenderer);

		Ekans_DifficultyMenu();
		break;
	case PLAYING:
		SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
		SDL_RenderClear(gRenderer);

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
		Ekans_GameOverScreen();
		break;
	case HIGHSCORES:
		SDL_RenderCopy(gRenderer, gEkansTempTexture, NULL, NULL);
		
		if(!gScreenFade) break;

		if(gJoypadPressed & (JOY_A | JOY_B | JOY_START)){
			gRenderFunc = Render_FadeTransition;
			gRenderFuncAfterFade = Ekans_RenderFunc;
			gEkansState = MAIN_MENU;
		}

		break;
	default:
		break;
	}
}

void Ekans_StopFunc(void){
	Mix_FreeMusic(gEkansIngameMusic);
	Mix_FreeMusic(gEkansTitlescreenMusic);
	SDL_DestroyTexture(gEkansTempTexture);
	GameExit();
}

void Ekans_StartGame(void){
	gEkansState = PLAYING;
	gEkansDirection = LEFT;
	gEkansScore = 0;
	gEkansVScore = 0;
	gEkansFramesSinceLastUpdate = 0;
	gEkansInputForNextUpdate = NONE;
	gEkansLength = 1;

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

	Mix_PlayMusic(gEkansIngameMusic, -1);
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
		gEkansScore += 3 * gEkansLength
			* gEkansScoreMultiplier[gEkansDifficulty];
		gEkansScore += rand() % 9 + 1;

		if(gEkansScore > 999999 || gEkansScore < 0){
			gEkansScore = 999999;
		}

		Ekans_AddSegment(newHeadX, newHeadY);
		Ekans_RandomizeFruitLocation();
	}else{
		Ekans_Segment * newHead = gEkansTail;
		newHead->x = newHeadX;
		newHead->y = newHeadY;

		gEkansHead->prev = newHead;
		gEkansTail = newHead->prev;
		newHead->prev->next = NULL;
		newHead->prev = NULL;
		newHead->next = gEkansHead;
		gEkansHead = newHead;

		Ekans_Segment * seg = Ekans_IsOccupied(newHeadX, newHeadY);
		if(seg && seg != gEkansHead) Ekans_GameOver();
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

	gEkansLength++;
}

Ekans_Segment * Ekans_IsOccupied(int x, int y){
	Ekans_Segment * curr = gEkansTail;

	if(Ekans_IsWall(x, y)){
		// return a pointer to Ekans_IsOccupied, and therefore
		// definitely not a segment
		return (Ekans_Segment *) Ekans_IsOccupied;
	}

	while(curr){
		if(curr->x == x && curr->y == y){
			return curr;
		}

		curr = curr->prev;
	}

	return NULL;
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

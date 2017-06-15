#ifndef _EKANS_H
#define _EKANS_H
#define EKANS_PLAYFIELD_WIDTH 40
#define EKANS_PLAYFIELD_HEIGHT 28
#define EKANS_HIGHSCORE_NAME_LENGTH 7
#define EKANS_NUM_HIGHSCORES 10
#include "global.h"

typedef enum { SLOW, NORMAL, FAST, INSANE } ekans_difficulty_t;
typedef enum { UP, DOWN, LEFT, RIGHT, NONE } ekans_direction_t;

typedef struct __Ekans_Segment {
	struct __Ekans_Segment * prev; // NULL if head
	struct __Ekans_Segment * next; // NULL if tail
	int x, y;
} Ekans_Segment;

typedef struct {
	char name[EKANS_HIGHSCORE_NAME_LENGTH + 1];
	int score;
} Ekans_ScoresTableEntry;

ekans_direction_t gEkansInputForNextUpdate;
ekans_direction_t gEkansDirection;
ekans_difficulty_t gEkansDifficulty;
enum { MAIN_MENU, DIFFICULTY, PLAYING, PAUSED, GAME_OVER,
	HIGHSCORES, SAVE_HIGHSCORE } gEkansState;
int gEkansLength;
int gEkansScore;
int gEkansVScore;
int gEkansVHighScore;
int gEkansFruitX;
int gEkansFruitY;
int gEkansMenuCursorLocation;
int gEkansFramesSinceLastUpdate;
int gEkansGameOverFade;
int gEkansScoreIndex; // set to the index to gEkansHighscores
					// when entering the name
SDL_Texture * gEkansTempTexture;
Ekans_Segment * gEkansHead;
Ekans_Segment * gEkansTail;
Ekans_ScoresTableEntry gEkansHighscores[EKANS_NUM_HIGHSCORES];

void Ekans_StartFunc(void);
void Ekans_RenderFunc(void);
void Ekans_StopFunc(void);
void Ekans_StartGame(void);
void Ekans_RenderPlayfield(void);
void Ekans_RenderScore(void);
void Ekans_RenderWalls(void);
void Ekans_LogicUpdate(void);
void Ekans_ProcessInput(void);
void Ekans_AddSegment(int x, int y);
bool Ekans_IsOccupied(int x, int y);
bool Ekans_IsWall(int x, int y);
void Ekans_RandomizeFruitLocation(void);

void Ekans_MainMenu(void);
void Ekans_DifficultyMenu(void);
void Ekans_DrawLogo(void);

void Ekans_GameOver(void);
void Ekans_GameOverScreen(void);
void Ekans_RenderHighscores(char * header, bool newHighscore);
void Ekans_LoadHighscores(void);
void Ekans_SaveHighscores(void);
#endif

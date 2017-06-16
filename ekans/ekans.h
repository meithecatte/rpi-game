#ifndef _EKANS_H
#define _EKANS_H
#define EKANS_PLAYFIELD_WIDTH 40
#define EKANS_PLAYFIELD_HEIGHT 28
#define EKANS_SCORE_NAME_LENGTH 7
#define EKANS_NUM_SCORES 10
#define EKANS_SAVEDATA_SIZE 128
#include "global.h"

typedef enum { SLOW, NORMAL, FAST, INSANE } ekans_difficulty_t;
typedef enum { UP, DOWN, LEFT, RIGHT, NONE } ekans_direction_t;
typedef enum { MAIN_MENU, DIFFICULTY, PLAYING, PAUSED, GAME_OVER,
	HIGHSCORES } ekans_state_t;

typedef struct __Ekans_Segment {
	struct __Ekans_Segment * prev; // NULL if head
	struct __Ekans_Segment * next; // NULL if tail
	int x, y;
} Ekans_Segment;

typedef struct {
	char name[EKANS_SCORE_NAME_LENGTH + 1];
	int score;
} Ekans_ScoresTableEntry;

extern ekans_direction_t gEkansInputForNextUpdate;
extern ekans_direction_t gEkansDirection;
extern ekans_difficulty_t gEkansDifficulty;
extern ekans_state_t gEkansState;
extern int gEkansLength;
extern int gEkansScore;
extern int gEkansVScore;
extern int gEkansVHighScore;
extern int gEkansFruitX;
extern int gEkansFruitY;
extern int gEkansMenuCursorLocation;
extern int gEkansFramesSinceLastUpdate;
extern int gEkansGameOverFade;
extern int gEkansScoreIndex;
extern int gEkansNameEntryBlinkCounter;
extern char gEkansScoreName[EKANS_SCORE_NAME_LENGTH + 1];
extern bool gEkansNameEntryFinished;
extern SDL_Texture * gEkansTempTexture;
extern Ekans_Segment * gEkansHead;
extern Ekans_Segment * gEkansTail;
extern Ekans_ScoresTableEntry gEkansHighscores[EKANS_NUM_SCORES];

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
void Ekans_FinishNameEntry(void);
void Ekans_RenderHighscores(char * header, char * subHeader);
void Ekans_LoadHighscores(void);
void Ekans_SaveHighscores(void);
#endif

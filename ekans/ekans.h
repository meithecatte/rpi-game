#ifndef _EKANS_H
#define _EKANS_H
#define EKANS_PLAYFIELD_WIDTH 20
#define EKANS_PLAYFIELD_HEIGHT 14
#define EKANS_HIGHSCORE_NAME_LENGTH 7
#include "helper.h"

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

typedef struct {
	ekans_direction_t inputForNextUpdate;
	ekans_direction_t direction;
	ekans_difficulty_t difficulty;
	enum { MAIN_MENU, DIFFICULTY, PLAYING, PAUSED, GAME_OVER,
		HIGHSCORES, SAVE_HIGHSCORE } state;
	int length;
	int score;
	int displayedScore;
	int displayedHighScore;
	int fruitX;
	int fruitY;
	int menuCursorLocation;
	int framesSinceLastUpdate;
	Ekans_Segment * head;
	Ekans_Segment * tail;
	Ekans_ScoresTableEntry highScores[10];
} ekans_state_t;

extern ekans_state_t gEkansState;

void Ekans_StartFunc(void);
void Ekans_RenderFunc(void);
void Ekans_StartGame(void);
void Ekans_RenderPlayfield(void);
void Ekans_LogicUpdate(void);
void Ekans_ProcessInput(void);
void Ekans_AddSegment(int x, int y);
bool Ekans_IsOccupied(int x, int y);
void Ekans_RandomizeFruitLocation(void);

void Ekans_MainMenu(void);
void Ekans_DifficultyMenu(void);

void Ekans_LoadHighscores(void);
void Ekans_SaveHighscores(void);
#endif

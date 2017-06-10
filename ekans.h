#ifndef _EKANS_H
#define _EKANS_H
#define EKANS_PLAYFIELD_WIDTH 20
#define EKANS_PLAYFIELD_HEIGHT 14

typedef enum { SLOW, NORMAL, FAST, INSANE } ekans_difficulty_t;
typedef enum { UP, DOWN, LEFT, RIGHT, NONE } ekans_direction_t;

typedef struct __Ekans_Segment {
	struct __Ekans_Segment * prev; // NULL if head
	struct __Ekans_Segment * next; // NULL if tail
	u16 x, y;
} Ekans_Segment;

typedef struct {
	char name[4]; // 3 + NULL
	int score;
} Ekans_ScoresTableEntry;

typedef struct {
	Ekans_Segment * head;
	Ekans_Segment * tail;
	int score;
	int menuCursorLocation;
	u8 framesPerUpdate;
	u8 framesSinceLastUpdate;
	ekans_direction_t inputForNextUpdate;
	ekans_direction_t direction;
	ekans_difficulty_t difficulty;
	enum { MAIN_MENU, DIFFICULTY, PLAYING, PAUSED, HIGHSCORES, SAVE_HIGHSCORE } state;
	u16 fruitX;
	u16 fruitY;
	Ekans_ScoresTableEntry highScoreTable[10];
} ekans_state_t;

extern const ekans_direction_t gEkansOppositeDirection[];
extern ekans_state_t gEkansState;

void Ekans_StartFunction(void);
void Ekans_RenderFunction(void);
void Ekans_StopFunction(void);

void Ekans_LoadHighscores(void);
void Ekans_SaveHighscores(void);
void Ekans_LogicUpdate(void);
#endif

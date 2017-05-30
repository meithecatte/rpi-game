#ifndef _EKANS_H
#define _EKANS_H

typedef enum { SLOW, NORMAL, FAST, INSANE } ekans_difficulty_t;
typedef enum { MAIN_MENU, PLAYING, PAUSED, HIGHSCORES, SAVE_HIGHSCORE } ekans_state_t;
typedef enum { UP, DOWN, LEFT, RIGHT } ekans_direction_t;

extern const ekans_direction_t gEkansOppositeDirection[];

typedef struct __Ekans_Segment {
	struct __Ekans_Segment * prev; // NULL if head
	struct __Ekans_Segment * next; // NULL if tail
} Ekans_Segment;

typedef struct {
	char name[4]; // 3 + NULL
	int score;
} Ekans_ScoresTableEntry;

void Ekans_StartFunction(void);
void Ekans_RenderFunction(void);
void Ekans_StopFunction(void);

void Ekans_LoadHighscores(void);
void Ekans_SaveHighscores(void);
void Ekans_LogicUpdate(void);

#endif

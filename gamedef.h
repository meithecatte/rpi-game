#ifndef _GAMEDEF_H
#define _GAMEDEF_H
#define GAME_COUNT 2
#include <SDL2/SDL.h>

typedef void (*void_func_t)(void);

typedef enum { GAME_POWEROFF, GAME_EKANS, GAME_NONE } game_index_t;

typedef struct {
	void_func_t startFunction;
	void_func_t renderFunction;
	char * menuNameTop;
	char * menuNameBottom;
	char * menuIconPath;
	SDL_Texture * menuIcon;
	SDL_Texture * menuRendered;
} game_t;

extern game_t gGames[GAME_COUNT];
#endif

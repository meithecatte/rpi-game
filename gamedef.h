#ifndef _GAMEDEF_H
#define _GAMEDEF_H
#define GAME_COUNT 1

typedef void (*start_func_t)(void);
typedef void (*render_func_t)(void);

typedef enum { GAME_EKANS, GAME_NONE } game_index_t;

typedef struct {
	start_func_t startFunction;
	render_func_t renderFunction;
	char * menuNameTop;
	char * menuNameBottom;
	char * menuIconPath;
	SDL_Texture * menuIcon;
	SDL_Texture * menuRendered;
} game_t;

extern game_t gGames[GAME_COUNT];
#endif

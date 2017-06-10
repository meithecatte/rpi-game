#ifndef _GAMEDEF_H
#define _GAMEDEF_H
#define GAME_COUNT 1

typedef void (*start_func_t)(void);	// when the game is opened
typedef void (*render_func_t)(void);	// every frame while the game is playing

typedef enum { GAME_EKANS = 0, GAME_NONE } game_index_t;

typedef struct {
	game_index_t gameIndex;
	start_func_t startFunction;
	render_func_t renderFunction;
	char * menuNameTop;
	char * menuNameBottom;
	char * menuIconPath;
	SDL_Texture * menuIcon;
} game_t;

extern game_t gGames[GAME_COUNT];
#endif

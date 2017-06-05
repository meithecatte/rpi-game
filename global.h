#ifndef _GLOBAL_H
#define _GLOBAL_H
#include <stdint.h>
#include <stdio.h>
#include <errno.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#include "ekans.h"

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;

typedef void (*init_func_t)(void);	// at bootup
typedef void (*start_func_t)(void);	// when the game is opened
typedef void (*render_func_t)(void);	// every frame while the game is playing
typedef void (*stop_func_t)(void);	// when the game is closed
typedef void (*cleanup_func_t)(void);	// at shutdown

typedef enum { GAME_EKANS = 0, GAME_NONE } game_index_t;
typedef enum { KEY_NONE, KEY_BLACK } color_key_index_t;

typedef union {
	struct {
		u8 frameCounter;
	} splash;

	struct {
		u16 scrollOffset;
		game_index_t currentGame;
		game_index_t fireUp;
	} gameSelectMenu;

	struct {
		Ekans_Segment * head;
		Ekans_Segment * tail;
		int score;
		int menuCursorLocation;
		u8 framesPerUpdate;
		u8 framesSinceLastUpdate;
		ekans_difficulty_t difficulty;
		ekans_state_t state;
		ekans_direction_t direction;
		u16 fruitX;
		u16 fruitY;
		Ekans_ScoresTableEntry highScoreTable[4][10]; // per difficulty, [diff][index]
	} ekans;
} render_state_t;

typedef struct {
	game_index_t gameIndex;
	init_func_t initFunction;
	start_func_t startFunction;
	render_func_t renderFunction;
	stop_func_t stopFunction;
	cleanup_func_t cleanupFunction;
	char * menuNameTop;
	char * menuNameBottom;
	char * menuIconPath;
	SDL_Texture * menuIcon;
} game_t;

#define ERROR_ON_SDL(cond,mesg) if(cond){fprintf(stderr,mesg " error: %s\n", SDL_GetError());exit(1);}
#define ERROR_ON_IMG(cond,mesg) if(cond){fprintf(stderr,mesg " error: %s\n", IMG_GetError());exit(1);}
#define ERROR_ON_MIX(cond,mesg) if(cond){fprintf(stderr,mesg " error: %s\n", Mix_GetError());exit(1);}
#define ERROR_ON_SYS(cond,mesg) if(cond){fprintf(stderr,mesg " error (%d): %s\n", errno, strerror(errno));exit(1);}
#define CALL_UNLESS_NULL(func) if(func) func()

#define JOY_SELECT 0x200
#define JOY_START 0x100
#define JOY_UP 0x080
#define JOY_DOWN 0x040
#define JOY_LEFT 0x020
#define JOY_RIGHT 0x010
#define JOY_A 0x008
#define JOY_B 0x800
#define JOY_X 0x004
#define JOY_Y 0x400
#define JOY_L 0x002
#define JOY_R 0x001

#define GAME_COUNT 1
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

#define IMAGE_LIST \
	X(gTextureFont8, "assets/font8.png", KEY_BLACK)				\
	X(gTextureFont16, "assets/font16.png", KEY_BLACK)			\
	X(gTextureSplash, "assets/splash.png", KEY_NONE)			\
	X(gTextureMenuBackground, "assets/menu_background.png", KEY_NONE)

// main.c
void cleanup(void);
void Render_DoFPS(void);
void Render_SplashScreen(void);
void Render_GameSelectMenu_RenderGame(const game_t * game, int dx);
void Render_GameSelectMenu(void);

extern SDL_Window * gWindow;
extern SDL_Renderer * gRenderer;
extern SDL_Texture * gScreen;
extern u8 gScreenFade;
extern render_func_t gRenderFunc;
extern render_state_t gRenderState;
extern u16 gJoypadHeld, gJoypadPressed;

#define X(var,path,key) extern SDL_Texture * var;
IMAGE_LIST
#undef X

// joypad.c
void InitJoypad(void);
u16 ReadJoypad(void);

// gamedef.c
extern game_t gGames[GAME_COUNT];

// helper.c
#define RenderChar8(x,y,c) RenderChar(x,y,c,8)
#define RenderChar16(x,y,c) RenderChar(x,y,c,16)

#define RenderText8(x,y,s) RenderText(x,y,s,8)
#define RenderText16(x,y,s) RenderText(x,y,s,16)

#define SetFontColor8(r,g,b) SDL_SetTextureColorMod(gTextureFont8, r, g, b);
#define SetFontColor16(r,g,b) SDL_SetTextureColorMod(gTextureFont16, r, g, b);

SDL_Texture * loadTexture(const char * path, color_key_index_t key);
void RenderChar(int x, int y, char c, int height);
void RenderText(int x, int y, const char * s, int height);
void Render_FadeIn(void);
void Render_FadeTransition(void);

extern render_func_t gRenderFuncAfterFade;
#endif

#ifndef _GLOBAL_H
#define _GLOBAL_H
#include <stdint.h>
#include <stdio.h>
#include <errno.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;

typedef void (*render_func_t)(void);

typedef union {
	struct {
		u8 frameCounter;
	} splash;
} render_state_t;

void Render_SplashScreen(void);

#define ERROR_ON_SDL(cond,mesg) if(cond){fprintf(stderr,mesg " error: %s\n", SDL_GetError());exit(1);}
#define ERROR_ON_IMG(cond,mesg) if(cond){fprintf(stderr,mesg " error: %s\n", IMG_GetError());exit(1);}
#define ERROR_ON_MIX(cond,mesg) if(cond){fprintf(stderr,mesg " error: %s\n", Mix_GetError());exit(1);}
#define ERROR_ON_SYS(cond,mesg) if(cond){fprintf(stderr,mesg " error (%d): %s\n", errno, strerror(errno));exit(1);}

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

// main.c
void cleanup(void);
SDL_Texture * loadTexture(char * path);

// joypad.c
void init_joypad(void);
u16 read_joypad(void);

#endif

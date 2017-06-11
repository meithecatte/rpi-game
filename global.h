#ifndef _GLOBAL_H
#define _GLOBAL_H
#define _GNU_SOURCE
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define IMAGE_LIST \
	X(gTextureFont8, "assets/font8.png", KEY_BLACK)				\
	X(gTextureFont16, "assets/font16.png", KEY_BLACK)			\
	X(gTextureSplash, "assets/splash.png", KEY_NONE)			\
	X(gTextureMenuBackground, "assets/menu_background.png", KEY_NONE)

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

#include "gamedef.h"
#include "joypad.h"
#include "helper.h"
#include "main.h"
#include "ui.h"
#endif

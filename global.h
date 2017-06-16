#ifndef _GLOBAL_H
#define _GLOBAL_H
#define _GNU_SOURCE
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define STORAGE_PATH "/var/local/game/"
#define IMAGE_LIST \
	X(gTextureFont8, "assets/font8.png", KEY_BLACK)				\
	X(gTextureFont16, "assets/font16.png", KEY_BLACK)			\
	X(gTextureSplash, "assets/splash.png", KEY_NONE)			\
	X(gTextureMenuBackground, "assets/menu_background.png", KEY_NONE) \
	X(gEkansIngameLogo, "assets/ekans/ingame_logo.png", KEY_NONE) \
	X(gEkansFruit, "assets/ekans/fruit.png", KEY_WHITE) \
	X(gEkansSegments, "assets/ekans/snake_segments.png", KEY_WHITE)

#include <stdbool.h>
#include <stdint.h>
#include "main.h"
#include "helper.h"
void GameExit(void);
#endif

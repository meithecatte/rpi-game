#include "global.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#define FADE_SPEED 16

void_func_t gRenderFuncAfterFade = NULL;
int gMallocCount;

SDL_Texture * Load_Texture(const char * path,
		color_key_index_t key){
	printf("Loading %s\n", path);

	SDL_Surface * surface = IMG_Load(path);
	ERROR_ON_IMG(surface == NULL, "IMG_Load");

	switch(key){
	case KEY_NONE:
		break;
	case KEY_BLACK:
		SDL_SetColorKey(surface, SDL_TRUE,
			SDL_MapRGB(surface->format, 0, 0, 0));
		break;
	case KEY_WHITE:
		SDL_SetColorKey(surface, SDL_TRUE,
			SDL_MapRGB(surface->format, 255, 255, 255));
		break;
	}

	SDL_Texture * texture =
		SDL_CreateTextureFromSurface(gRenderer, surface);
	ERROR_ON_SDL(texture == NULL, "SDL_CreateTextureFromSurface");

	SDL_FreeSurface(surface);

	return texture;
}

Mix_Music* Load_Music(const char* path){
	printf("Loading %s\n", path);
	Mix_Music* music = Mix_LoadMUS(path);
	ERROR_ON_MIX(!music, "Mix_LoadMUS");
	return music;
}

void * _safe_malloc(size_t size, const char * error){
	void * ptr = malloc(size); // TEST_EXCEPTION

	if(ptr == NULL){
		fprintf(stderr, "%s\n", error);
		exit(1);
	}

	gMallocCount++;
	return ptr;
}

void _free(void * ptr){
	gMallocCount--;
	free(ptr); // TEST_EXCEPTION
}

void RenderChar(int x, int y, int scale,
	char c, int height){
	SDL_Rect srcrect = { .x = (c % 16) * 8, .y = (c / 16) * height,
		.w = 8, .h = height };
	SDL_Rect dstrect = { .x = x, .y = y,
		.w = 8 * scale, .h = height * scale };
	SDL_RenderCopy(gRenderer,
		(height == 8) ? gTextureFont8 : gTextureFont16,
		&srcrect, &dstrect);
}

void RenderText(int x, int y, int scale,
	const char * s, int height){
	while(*s){
		RenderChar(x, y, scale, *s++, height);
		x += 8 * scale;
	}
}

void Render_FadeIn(void){
	if(gScreenFade < 255 - FADE_SPEED) gScreenFade += FADE_SPEED;
	else gScreenFade = 255;

	if(gScreenFade == 255){
		gRenderFunc = gRenderFuncAfterFade;
	}

	gRenderFuncAfterFade();
}

void Render_FadeTransition(void){
	if(gScreenFade > FADE_SPEED) gScreenFade -= FADE_SPEED;
	else gScreenFade = 0;

	if(gScreenFade == 0){
		gRenderFunc = Render_FadeIn;
		gRenderFuncAfterFade();
	}
}

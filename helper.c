#include "global.h"
#define FADE_SPEED 16

render_func_t gRenderFuncAfterFade = NULL;
int gMallocCount;

SDL_Texture * Load_Texture(const char * path, color_key_index_t key){
	printf("Loading %s\n", path);

	SDL_Surface * surface = IMG_Load(path);
	ERROR_ON_IMG(surface == NULL, "IMG_Load");

	switch(key){
	case KEY_NONE:
		break;
	case KEY_BLACK:
		SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0, 0, 0));
		break;
	}

	SDL_Texture * texture = SDL_CreateTextureFromSurface(gRenderer, surface);
	ERROR_ON_SDL(texture == NULL, "SDL_CreateTextureFromSurface");

	SDL_FreeSurface(surface);

	return texture;
}

void * _safe_malloc(size_t size, char * error){
	void * ptr = malloc(size); // TEST_EXCEPTION

	if(ptr == NULL){
		fprintf(stderr, error);
		exit(1);
	}

	gMallocCount++;
	return ptr;
}

void _free(void * ptr){
	gMallocCount--;
	free(ptr); // TEST_EXCEPTION
}

void RenderChar(int x, int y, char c, int height){
	SDL_Rect srcrect = { .x = (c % 16) * 8, .y = (c / 16) * height, .w = 8, .h = height };
	SDL_Rect dstrect = { .x = x, .y = y, .w = 8, .h = height };
	SDL_RenderCopy(gRenderer, (height == 8) ? gTextureFont8 : gTextureFont16, &srcrect, &dstrect);
}

void RenderText(int x, int y, const char * s, int height){
	while(*s){
		RenderChar(x, y, *s++, height);
		x += 8;
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

#include "global.h"

render_func_t gRenderFuncAfterFade = NULL;

SDL_Texture * loadTexture(char * path, color_key_index_t key){
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

void RenderChar(int x, int y, char c){
	SDL_Rect srcrect = { .x = (c % 16) * 8, .y = (c / 16) * 16, .w = 8, .h = 16 };
	SDL_Rect dstrect = { .x = x, .y = y, .w = 8, .h = 16 };
	SDL_RenderCopy(gRenderer, gTextureFont, &srcrect, &dstrect);
}

void RenderText(int x, int y, char * s){
	while(*s){
		RenderChar(x, y, *s++);
		x += 8;
	}
}

void Render_FadeIn(void){
	if(gScreenFade < 255 - FADE_SPEED) gScreenFade += FADE_SPEED;
	else if(gScreenFade != 255) gScreenFade = 255;

	if(gScreenFade == 255){
		gRenderFunc = gRenderFuncAfterFade;
	}

	gRenderFuncAfterFade();
}

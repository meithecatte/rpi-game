#include "ekans.h"
#include <math.h>

void Ekans_RenderPlayfield(void){
	Ekans_RenderWalls();
	Ekans_RenderScore();
	SDL_Rect dstrect;
	dstrect.w = 16;
	dstrect.h = 16;

	SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
	dstrect.x = gEkansFruitX * 16;
	dstrect.y = gEkansFruitY * 16;
	SDL_RenderFillRect(gRenderer, &dstrect);
	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);

	Ekans_Segment * curr = gEkansTail;
	while(curr){
		if(curr == gEkansHead){
			SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
		}

		dstrect.x = curr->x * 16;
		dstrect.y = curr->y * 16;
		SDL_RenderFillRect(gRenderer, &dstrect);
		curr = curr->prev;
	}
}

void Ekans_RenderScore(void){
	SDL_Rect dstrect;

	dstrect.x = 0;
	dstrect.y = SCREEN_HEIGHT - 32;
	dstrect.w = SCREEN_WIDTH;
	dstrect.h = 1;
	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
	SDL_RenderFillRect(gRenderer, &dstrect);

	gEkansVScore += ceilf((gEkansScore - gEkansVScore) / 10.f);

	if(gEkansVScore > gEkansVHighScore){
		gEkansVHighScore = gEkansVScore;
	}

	char buffer[39];
	sprintf(buffer, "Score: %06d        High Score: %06d",
		gEkansVScore, gEkansVHighScore);
	RenderText8s(8, SCREEN_HEIGHT - 24, 2, buffer);
}

void Ekans_RenderWalls(void){
	SDL_Rect dstrect;

	SDL_SetRenderDrawColor(gRenderer, 0, 0, 255, 255);
	dstrect.x = 0;
	dstrect.y = 0;
	dstrect.w = 6 * 16;
	dstrect.h = 16;
	SDL_RenderFillRect(gRenderer, &dstrect);
	dstrect.x = (EKANS_PLAYFIELD_WIDTH - 6) * 16;
	SDL_RenderFillRect(gRenderer, &dstrect);
	dstrect.y = (EKANS_PLAYFIELD_HEIGHT - 1) * 16;
	SDL_RenderFillRect(gRenderer, &dstrect);
	dstrect.x = 0;
	SDL_RenderFillRect(gRenderer, &dstrect);
	dstrect.x = 0;
	dstrect.y = 16;
	dstrect.w = 16;
	dstrect.h = 5 * 16;
	SDL_RenderFillRect(gRenderer, &dstrect);
	dstrect.x = (EKANS_PLAYFIELD_WIDTH - 1) * 16;
	SDL_RenderFillRect(gRenderer, &dstrect);
	dstrect.y = (EKANS_PLAYFIELD_HEIGHT - 6) * 16;
	SDL_RenderFillRect(gRenderer, &dstrect);
	dstrect.x = 0;
	SDL_RenderFillRect(gRenderer, &dstrect);
}

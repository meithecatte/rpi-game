#include "ekans.h"
#include <math.h>

void Ekans_RenderPlayfield(void){
	Ekans_RenderWalls();
	Ekans_RenderScore();
	SDL_Rect srcrect, dstrect;
	dstrect.w = 16;
	dstrect.h = 16;
	dstrect.x = gEkansFruitX * 16;
	dstrect.y = gEkansFruitY * 16;
	SDL_RenderCopy(gRenderer, gEkansFruit, NULL, &dstrect);

	srcrect.w = 16;
	srcrect.h = 16;

	Ekans_Segment * curr = gEkansTail;
	while(curr){
		dstrect.x = curr->x * 16;
		dstrect.y = curr->y * 16;
		srcrect.x = Ekans_GetSegmentDirection(curr) * 16;
		srcrect.y = Ekans_GetSegmentType(curr) * 16;
		SDL_RenderCopy(gRenderer, gEkansSegments,
			&srcrect, &dstrect);
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

ekans_direction_t Ekans_GetSegmentDirection(
		const Ekans_Segment * segment){
	if(!segment->prev){
		return gEkansDirection;
	}

	int dx = (segment->prev->x - segment->x);
	int dy = (segment->prev->y - segment->y);

	if(dx < -1) dx += EKANS_PLAYFIELD_WIDTH;
	if(dx > 1) dx -= EKANS_PLAYFIELD_WIDTH;
	if(dy < -1) dy += EKANS_PLAYFIELD_HEIGHT;
	if(dy > 1) dy -= EKANS_PLAYFIELD_HEIGHT;

	if(dx == 1) return RIGHT;
	if(dx == -1) return LEFT;
	if(dy == 1) return DOWN;
	if(dy == -1) return UP;
	
	fprintf(stderr, "Ekans_GetSegmentDirection: noreach?\n");
	exit(1);
}

ekans_segment_type_t Ekans_GetSegmentType(
		const Ekans_Segment * segment){
	if(!segment->prev){
		return HEAD;
	}

	if(!segment->next){
		return TAIL;
	}

	ekans_direction_t dir, dirNext;
	dir = Ekans_GetSegmentDirection(segment);
	dirNext = Ekans_GetSegmentDirection(segment->next);

	if(dir == dirNext){
		return STRAIGHT;
	}

	switch(dir){
	case UP:    return (dirNext == RIGHT) ? LEFTTURN : RIGHTTURN;
	case DOWN:  return (dirNext == LEFT)  ? LEFTTURN : RIGHTTURN;
	case LEFT:  return (dirNext == UP)    ? LEFTTURN : RIGHTTURN;
	case RIGHT: return (dirNext == DOWN)  ? LEFTTURN : RIGHTTURN;
	default:
		fprintf(stderr, "Ekans_GetSegmentType: no direction?\n");
		exit(1);
	}
}

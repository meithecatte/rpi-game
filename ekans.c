#include "global.h"
#include "ekans.h"

const ekans_direction_t gEkansOppositeDirection[] = {
	[UP] = DOWN,
	[DOWN] = UP,
	[LEFT] = RIGHT,
	[RIGHT] = LEFT,
};

void Ekans_StartFunction(void){
	gRenderState.ekans.score = 0;
	gRenderState.ekans.difficulty = NORMAL;
	gRenderState.ekans.state = MAIN_MENU;
	gRenderState.ekans.head = NULL;
	gRenderState.ekans.tail = NULL;
}

void Ekans_RenderFunction(void){
	;
}

void Ekans_StopFunction(void){
	;
}

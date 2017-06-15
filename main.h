#ifndef _MAIN_H
#define _MAIN_H
#include "gamedef.h"
#include <SDL2/SDL.h>
extern SDL_Renderer * gRenderer;
extern SDL_Texture * gScreen;
extern uint8_t gScreenFade;
extern render_func_t gRenderFunc;

#define X(var,path,key) extern SDL_Texture * var;
IMAGE_LIST
#undef X

void Load_Resources(void);
void Dealloc_Resources(void);
void Init_Random(void);
void Init_SDL(void);
void Init_FPS(void);
void Measure_FPS(void);
#endif

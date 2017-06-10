#ifndef _MAIN_H
#define _MAIN_H
extern SDL_Window * gWindow;
extern SDL_Renderer * gRenderer;
extern SDL_Texture * gScreen;
extern u8 gScreenFade, gExit;
extern render_func_t gRenderFunc;

#define X(var,path,key) extern SDL_Texture * var;
IMAGE_LIST
#undef X

int main(void);
void Load_Resources(void);
void Dealloc_Resources(void);
void Init_Random(void);
void Init_SDL(void);
void Init_FPS(void);
void Measure_FPS(void);
#endif

#ifndef _HELPER_H
#define _HELPER_H
#define CALL_UNLESS_NULL(func) if(func) func()
#define LENGTH(arr) ((int)(sizeof((arr))/sizeof((arr)[0])))
#define STR(x) STR2(x)
#define STR2(x) #x
#include <stdio.h>
#include <errno.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "gamedef.h"

#define ERROR_ON_SDL(cond,mesg) if(cond){ \
	fprintf(stderr,__FILE__ ":" STR(__LINE__) ": " mesg " error: %s\n", SDL_GetError());exit(1);}

#define ERROR_ON_IMG(cond,mesg) if(cond){ \
	fprintf(stderr,__FILE__ ":" STR(__LINE__) ": " mesg " error: %s\n", IMG_GetError());exit(1);}

#define ERROR_ON_MIX(cond,mesg) if(cond){ \
	fprintf(stderr,__FILE__ ":" STR(__LINE__) ": " mesg " error: %s\n", Mix_GetError());exit(1);}

#define ERROR_ON_SYS(cond,mesg) if(cond){ \
	fprintf(stderr,__FILE__ ":" STR(__LINE__) ": " mesg " error (%d): %s\n", errno, strerror(errno));exit(1);}

#define RenderChar8(x,y,c) RenderChar(x,y,1,c,8)
#define RenderChar8s(x,y,sc,c) RenderChar(x,y,sc,c,8)
#define RenderChar16(x,y,c) RenderChar(x,y,1,c,16)
#define RenderChar16s(x,y,sc,c) RenderChar(x,y,sc,c,16)

#define RenderText8(x,y,s) RenderText(x,y,1,s,8)
#define RenderText8s(x,y,sc,s) RenderText(x,y,sc,s,8)
#define RenderText16(x,y,s) RenderText(x,y,1,s,16)
#define RenderText16s(x,y,sc,s) RenderText(x,y,sc,s,16)

#define SetFontColor8(r,g,b) SDL_SetTextureColorMod(gTextureFont8, r, g, b);
#define SetFontColor16(r,g,b) SDL_SetTextureColorMod(gTextureFont16, r, g, b);

#define _malloc(size) _safe_malloc(size, __FILE__ ":" STR(__LINE__) ": out of memory")

typedef enum { KEY_NONE, KEY_BLACK, KEY_WHITE } color_key_index_t;

extern void_func_t gRenderFuncAfterFade;
extern int gMallocCount;

SDL_Texture* Load_Texture(const char* path, color_key_index_t key);
Mix_Music* Load_Music(const char* path);
// do not use _safe_malloc
// use the _malloc macro instead
void* _safe_malloc(size_t size, const char* error);
void _free(void* ptr);
void RenderChar(int x, int y, int scale, char c, int height);
void RenderText(int x, int y, int scale, const char* s, int height);
void Render_FadeIn(void);
void Render_FadeTransition(void);
#endif

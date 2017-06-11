#ifndef _HELPER_H
#define _HELPER_H
#define CALL_UNLESS_NULL(func) if(func) func()
#define LENGTH(arr) (sizeof((arr))/sizeof((arr)[0]))
#define STR(x) STR2(x)
#define STR2(x) #x

#define ERROR_ON_SDL(cond,mesg) if(cond){ \
	fprintf(stderr,__FILE__ ":" STR(__LINE__) ": " mesg " error: %s\n", SDL_GetError());exit(1);}

#define ERROR_ON_IMG(cond,mesg) if(cond){ \
	fprintf(stderr,__FILE__ ":" STR(__LINE__) ": " mesg " error: %s\n", IMG_GetError());exit(1);}

#define ERROR_ON_MIX(cond,mesg) if(cond){ \
	fprintf(stderr,__FILE__ ":" STR(__LINE__) ": " mesg " error: %s\n", Mix_GetError());exit(1);}

#define ERROR_ON_SYS(cond,mesg) if(cond){ \
	fprintf(stderr,__FILE__ ":" STR(__LINE__) ": " mesg " error (%d): %s\n", errno, strerror(errno));exit(1);}

#define RenderChar8(x,y,c) RenderChar(x,y,c,8)
#define RenderChar16(x,y,c) RenderChar(x,y,c,16)

#define RenderText8(x,y,s) RenderText(x,y,s,8)
#define RenderText16(x,y,s) RenderText(x,y,s,16)

#define SetFontColor8(r,g,b) SDL_SetTextureColorMod(gTextureFont8, r, g, b);
#define SetFontColor16(r,g,b) SDL_SetTextureColorMod(gTextureFont16, r, g, b);

#define _malloc(size) _safe_malloc(size, __FILE__ ":" STR(__LINE__) ": out of memory")

typedef enum { KEY_NONE, KEY_BLACK } color_key_index_t;

extern render_func_t gRenderFuncAfterFade;
extern int gMallocCount;

SDL_Texture * Load_Texture(const char * path, color_key_index_t key);
void * _safe_malloc(size_t size, char * error); // use the _malloc macro instead
void _free(void * ptr);
void RenderChar(int x, int y, char c, int height);
void RenderText(int x, int y, const char * s, int height);
void Render_FadeIn(void);
void Render_FadeTransition(void);
#endif

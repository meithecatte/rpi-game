#ifndef _UI_H
#define _UI_H
#include "gamedef.h"
void Render_SplashScreen(void);
void Render_GameSelectMenu(void);
void GameSelectMenu_PrepareBackground(void);
void GameSelectMenu_RenderGame(game_t * game);
#endif

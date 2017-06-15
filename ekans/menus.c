#include "ekans.h"
#include "joypad.h"
#define LOGO_WIDTH 192
#define LOGO_HEIGHT 192
#define MAIN_MENU_WIDTH (12 * 16) // longest option + cursor
#define DIFFICULTY_MENU_WIDTH (9 * 16)

void Ekans_MainMenu(void){
	const int menuX = (SCREEN_WIDTH - MAIN_MENU_WIDTH) / 2;
	const int menuY = LOGO_HEIGHT - 8;

	Ekans_DrawLogo();
	SetFontColor8(0, 0, 0);
	SetFontColor16(0, 0, 0);
	RenderChar8s(menuX, menuY + 20 * gEkansMenuCursorLocation,
		2, 0x01);
	RenderText8s(menuX + 16, menuY + 20 * 0, 2, "New Game");
	RenderText8s(menuX + 16, menuY + 20 * 1, 2, "High Scores");
	RenderText8s(menuX + 16, menuY + 20 * 2, 2, "Exit");

	if(!gScreenFade) return; // don't check input if mid-fade render

	if(gJoypadPressed & JOY_UP){
		if(gEkansMenuCursorLocation > 0){
			gEkansMenuCursorLocation--;
		}
	}else if(gJoypadPressed & JOY_DOWN){
		if(gEkansMenuCursorLocation < 2){
			gEkansMenuCursorLocation++;
		}
	}else if(gJoypadPressed & (JOY_START | JOY_A)){
		switch(gEkansMenuCursorLocation){
		case 0:
			gEkansState = DIFFICULTY;
			break;
		case 1:
			gRenderFunc = Render_FadeTransition;
			gRenderFuncAfterFade = Ekans_RenderFunc;
			gEkansState = HIGHSCORES;
			break;
		case 2:
			Ekans_StopFunc();
			GameExit();
			break;
		}
	}
}

void Ekans_DifficultyMenu(void){
	SDL_Rect dstrect;
	dstrect.x = (SCREEN_WIDTH - LOGO_WIDTH) / 2; // logo
	dstrect.y = 0;
	dstrect.w = LOGO_WIDTH;
	dstrect.h = LOGO_HEIGHT;
	SDL_RenderCopy(gRenderer, gGames[GAME_EKANS].menuIcon,
		NULL, &dstrect);

	const int menuX = (SCREEN_WIDTH - DIFFICULTY_MENU_WIDTH) / 2;
	const int menuY = LOGO_HEIGHT - 8 + 40;

	SetFontColor8(0, 0, 0);
	SetFontColor16(0, 0, 0);
	RenderText16s((SCREEN_WIDTH - 17 * 16) / 2, menuY - 40, 2,
		"Difficulty Select");
	RenderChar8s(menuX,
		menuY + 24 * gEkansDifficulty, 2, 0x06);
	RenderChar8s(menuX + 8 * 16,
		menuY + 24 * gEkansDifficulty, 2, 0x05);
	RenderText8s(menuX + 24 + 16, menuY + 24 * 0, 2,  "Slow");
	RenderText8s(menuX + 24,      menuY + 24 * 1, 2, "Normal");
	RenderText8s(menuX + 24 + 16, menuY + 24 * 2, 2,  "Fast");
	RenderText8s(menuX + 24,      menuY + 24 * 3, 2, "Insane");

	if(gJoypadPressed & JOY_UP){
		if(gEkansDifficulty > SLOW){
			gEkansDifficulty--;
		}
	}else if(gJoypadPressed & JOY_DOWN){
		if(gEkansDifficulty < INSANE){
			gEkansDifficulty++;
		}
	}else if(gJoypadPressed & (JOY_START | JOY_A)){
		Ekans_StartGame();
	}else if(gJoypadPressed & JOY_B){
		gEkansState = MAIN_MENU;
	}
}

void Ekans_DrawLogo(void){
	SDL_Rect dstrect;
	dstrect.x = (SCREEN_WIDTH - LOGO_WIDTH) / 2;
	dstrect.y = 0;
	dstrect.w = LOGO_WIDTH;
	dstrect.h = LOGO_HEIGHT;
	SDL_RenderCopy(gRenderer, gGames[GAME_EKANS].menuIcon,
		NULL, &dstrect);
}

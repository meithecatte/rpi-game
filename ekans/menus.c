#include "global.h"
#include "ekans.h"

void Ekans_MainMenu(void){
	SDL_Rect dstrect = { .x = 112, .y = 0, .w = 96, .h = 96 }; // logo
	SDL_RenderCopy(gRenderer, gGames[GAME_EKANS].menuIcon, NULL, &dstrect);

	SetFontColor8(0, 0, 0);
	RenderChar8(112, 100 + 12 * gEkansState.menuCursorLocation, 0x01); // cursor
	RenderText8(120, 100, "New Game"); // menu entries
	RenderText8(120, 112, "High Scores");
	RenderText8(120, 124, "Exit");

	if(!gScreenFade) return; // don't check input if mid-fade render

	if(gJoypadPressed & JOY_UP){
		if(gEkansState.menuCursorLocation > 0)
			gEkansState.menuCursorLocation--;
	}else if(gJoypadPressed & JOY_DOWN){
		if(gEkansState.menuCursorLocation < 2)
			gEkansState.menuCursorLocation++;
	}else if(gJoypadPressed & (JOY_START | JOY_A)){
		switch(gEkansState.menuCursorLocation){
		case 0:
			gEkansState.state = DIFFICULTY;
			break;
		case 2:
			GameExit();
			break;
		}
	}
}

void Ekans_DifficultyMenu(void){
	SDL_Rect dstrect = { .x = 112, .y = 0, .w = 96, .h = 96 }; // logo
	SDL_RenderCopy(gRenderer, gGames[GAME_EKANS].menuIcon, NULL, &dstrect);

	SetFontColor8(0, 0, 0);
	SetFontColor16(0, 0, 0);
	RenderChar8(124, 112 + 12 * gEkansState.difficulty, 0x06); // cursor
	RenderChar8(188, 112 + 12 * gEkansState.difficulty, 0x05);
	RenderText16(92, 88, "Difficulty Select"); // header
	RenderText8(144, 112, "Slow"); // menu entries
	RenderText8(136, 124, "Normal");
	RenderText8(144, 136, "Fast");
	RenderText8(136, 148, "Insane");

	if(gJoypadPressed & JOY_UP){
		if(gEkansState.difficulty > 0)
			gEkansState.difficulty--;
	}else if(gJoypadPressed & JOY_DOWN){
		if(gEkansState.difficulty < 3)
			gEkansState.difficulty++;
	}else if(gJoypadPressed & JOY_A){
		Ekans_StartGame();
	}else if(gJoypadPressed & JOY_B){
		gEkansState.state = MAIN_MENU;
	}
}

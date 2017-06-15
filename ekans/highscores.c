#include "ekans.h"
#include "joypad.h"
#include <string.h>

void Ekans_GameOver(void){
	gEkansState = GAME_OVER;
	gEkansVScore = gEkansScore;
	gEkansGameOverFade = 255;
	gEkansScoreIndex = -1;

	if(gEkansScore >
			gEkansHighscores[EKANS_NUM_HIGHSCORES - 1].score){
		for(int i = 0;i < EKANS_NUM_HIGHSCORES;i++){
			if(gEkansScore > gEkansHighscores[i].score){
				for(int j = EKANS_NUM_HIGHSCORES - 1;j > i;j--){
					strcpy(gEkansHighscores[j].name,
						gEkansHighscores[j - 1].name);
					gEkansHighscores[j].score =
						gEkansHighscores[j - 1].score;
				}

				gEkansScoreIndex = i;
				gEkansHighscores[i].score = gEkansScore;
				strcpy(gEkansHighscores[i].name, "YOU");
				break;
			}
		}
	}

	SDL_SetRenderTarget(gRenderer, gEkansTempTexture);
	SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
	SDL_RenderClear(gRenderer);
	Ekans_RenderPlayfield();
	SDL_SetRenderTarget(gRenderer, gScreen);
}

void Ekans_GameOverScreen(void){
	if(gEkansGameOverFade > 64){
		gEkansGameOverFade -= 16;

		SDL_SetTextureColorMod(gEkansTempTexture,
			gEkansGameOverFade, gEkansGameOverFade,
			gEkansGameOverFade);
		SDL_RenderCopy(gRenderer, gEkansTempTexture, NULL, NULL);
	}

	if(gEkansGameOverFade < 64){
		gEkansGameOverFade = 64;

		SDL_SetRenderTarget(gRenderer, gEkansTempTexture);
		SDL_RenderCopy(gRenderer, gScreen, NULL, NULL);
		Ekans_RenderHighscores("Game Over", gEkansScoreIndex >= 0);
		SDL_SetRenderTarget(gRenderer, gScreen);
		SDL_SetTextureColorMod(gEkansTempTexture, 255, 255, 255);
	}
	
	if(gEkansGameOverFade == 64){
		SDL_RenderCopy(gRenderer, gEkansTempTexture, NULL, NULL);

		if(gJoypadPressed & JOY_A){
			gEkansState = MAIN_MENU;
			Ekans_Segment * curr = gEkansTail;
			do {
				Ekans_Segment * next = curr->prev;
				_free(curr);
				curr = next;
			}while(curr);
		}
	}
}

void Ekans_RenderHighscores(char * header, bool newHighscore){
	const char * subHeader = "Please enter your name";
	char buffer[16];
	SetFontColor8(255, 255, 255);
	SetFontColor16(255, 255, 255);

	RenderText16s((SCREEN_WIDTH - strlen(header) * 16) / 2, 16,
		2, header);

	if(newHighscore){
		RenderText8s((SCREEN_WIDTH - strlen(subHeader) * 16) / 2,
			56, 2, subHeader);
	}

	for(int i = 0;i < EKANS_NUM_HIGHSCORES;i++){
		if(newHighscore && i == gEkansScoreIndex) continue;
		sprintf(buffer, "%-7s  %06d", gEkansHighscores[i].name,
			gEkansHighscores[i].score);

		RenderText8s((SCREEN_WIDTH - 16 * 16) / 2, 86 + i * 20, 2,
			buffer);
	}
}

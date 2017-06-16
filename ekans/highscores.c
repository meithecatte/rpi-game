#include "ekans.h"
#include "joypad.h"
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int gEkansGameOverFade, gEkansScoreIndex;
int gEkansNameEntryBlinkCounter;
SDL_Texture * gEkansTempTexture;
char gEkansScoreName[EKANS_SCORE_NAME_LENGTH + 1];
bool gEkansNameEntryFinished;

void Ekans_GameOver(void){
	gEkansState = GAME_OVER;
	gEkansVScore = gEkansScore;
	gEkansGameOverFade = 255;
	gEkansScoreIndex = -1;
	gEkansNameEntryFinished = true;

	if(gEkansScore >
			gEkansHighscores[EKANS_NUM_SCORES - 1].score){
		gEkansNameEntryFinished = false;
		for(int i = 0;i < EKANS_NUM_SCORES;i++){
			if(gEkansScore > gEkansHighscores[i].score){
				for(int j = EKANS_NUM_SCORES - 1;j > i;j--){
					strcpy(gEkansHighscores[j].name,
						gEkansHighscores[j - 1].name);
					gEkansHighscores[j].score =
						gEkansHighscores[j - 1].score;
				}

				gEkansScoreIndex = i;
				gEkansHighscores[i].score = gEkansScore;
				strcpy(gEkansHighscores[i].name, "");
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
		gEkansNameEntryBlinkCounter = 0;

		SDL_SetRenderTarget(gRenderer, gEkansTempTexture);
		SDL_RenderCopy(gRenderer, gScreen, NULL, NULL);
		Ekans_RenderHighscores("Game Over", (gEkansScoreIndex >= 0)
			? "Please enter your name" : NULL);
		SDL_SetRenderTarget(gRenderer, gScreen);
		SDL_SetTextureColorMod(gEkansTempTexture, 255, 255, 255);
	}
	
	if(gEkansGameOverFade == 64){
		SDL_RenderCopy(gRenderer, gEkansTempTexture, NULL, NULL);

		if(!gEkansNameEntryFinished){
			int nameLen = strlen(gEkansScoreName);
			char lastChar = gEkansScoreName[nameLen - 1];

			if(gJoypadPressed & JOY_UP){
				gEkansNameEntryBlinkCounter = 0;
				lastChar++;
				
				if(lastChar > 'Z') lastChar = 'A';
				gEkansScoreName[nameLen - 1] = lastChar;
			}else if(gJoypadPressed & JOY_DOWN){
				gEkansNameEntryBlinkCounter = 0;
				lastChar--;

				if(lastChar < 'A') lastChar = 'Z';
				gEkansScoreName[nameLen - 1] = lastChar;
			}else if(gJoypadPressed & JOY_B){
				gEkansNameEntryBlinkCounter = 0;
				if(nameLen != 1){
					nameLen--;
					gEkansScoreName[nameLen] = '\0';
				}
			}else if(gJoypadPressed & JOY_A){
				gEkansNameEntryBlinkCounter = 0;
				if(nameLen == EKANS_SCORE_NAME_LENGTH){
					Ekans_FinishNameEntry();
				}else{
					gEkansScoreName[nameLen] = 'A';
					nameLen++;
					gEkansScoreName[nameLen] = '\0';
				}
			}else if(gJoypadPressed & JOY_START){
				Ekans_FinishNameEntry();
			}

			lastChar = gEkansScoreName[nameLen - 1];

			if(gEkansNameEntryBlinkCounter >= 15){
				gEkansScoreName[nameLen - 1] = '\0';
			}
		
			RenderText8s((SCREEN_WIDTH - 15 * 16) / 2,
					86 + gEkansScoreIndex * 20, 2, gEkansScoreName);

			gEkansScoreName[nameLen - 1] = lastChar;
			gEkansNameEntryBlinkCounter++;
		
			if(gEkansNameEntryBlinkCounter >= 30){
				gEkansNameEntryBlinkCounter = 0;
			}
		}else{
			if(gJoypadPressed & (JOY_A | JOY_B | JOY_START)){
				gRenderFunc = Render_FadeTransition;
				gRenderFuncAfterFade = Ekans_RenderFunc;
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
}

void Ekans_FinishNameEntry(void){
	gEkansNameEntryFinished = true;
	strcpy(gEkansHighscores[gEkansScoreIndex].name,
		gEkansScoreName);

	SDL_SetRenderTarget(gRenderer, gScreen);
	SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
	SDL_RenderClear(gRenderer);
	Ekans_RenderPlayfield();
	SDL_SetRenderTarget(gRenderer, gEkansTempTexture);
	SDL_SetTextureColorMod(gScreen, 64, 64, 64);
	SDL_RenderCopy(gRenderer, gScreen, NULL, NULL);
	Ekans_RenderHighscores("Game Over", "New Highscore");
	SDL_SetRenderTarget(gRenderer, gScreen);
	SDL_SetTextureColorMod(gScreen, 255, 255, 255);
	SDL_RenderCopy(gRenderer, gEkansTempTexture, NULL, NULL);

	Ekans_SaveHighscores();
}

void Ekans_RenderHighscores(char * header, char * subHeader){
	char buffer[16];
	SetFontColor8(255, 255, 255);
	SetFontColor16(255, 255, 255);

	RenderText16s((SCREEN_WIDTH - strlen(header) * 16) / 2, 16,
		2, header);

	if(subHeader){
		RenderText8s((SCREEN_WIDTH - strlen(subHeader) * 16) / 2,
			56, 2, subHeader);
	}

	for(int i = 0;i < EKANS_NUM_SCORES;i++){
		if(gEkansHighscores[i].score == 0) continue;
		sprintf(buffer, "%-7s  %06d",
			gEkansHighscores[i].name, gEkansHighscores[i].score);

		RenderText8s((SCREEN_WIDTH - 15 * 16) / 2, 86 + i * 20, 2,
			buffer);
	}
}

void Ekans_LoadHighscores(void){
	int fd = open(STORAGE_PATH "ekans", O_RDONLY);
	if(fd < 0)
		return;

	char data[EKANS_SAVEDATA_SIZE];

	if(read(fd, data, EKANS_SAVEDATA_SIZE) != EKANS_SAVEDATA_SIZE){
		close(fd);
		return;
	}
	
	close(fd);
	
	for(int i = 0;i < EKANS_NUM_SCORES;i++){
		strcpy(gEkansHighscores[i].name, &data[12 * i]);
		memcpy(&gEkansHighscores[i].score,
			&data[12 * i + EKANS_SCORE_NAME_LENGTH + 1],
			sizeof(int));
	}

	strcpy(gEkansScoreName, &data[12 * EKANS_NUM_SCORES]);
}

void Ekans_SaveHighscores(void){
	char data[EKANS_SAVEDATA_SIZE];
	memset(data, 0, EKANS_SAVEDATA_SIZE);

	for(int i = 0;i < EKANS_NUM_SCORES;i++){
		strcpy(&data[12 * i], gEkansHighscores[i].name);
		memcpy(&data[12 * i + EKANS_SCORE_NAME_LENGTH + 1],
			&gEkansHighscores[i].score, sizeof(int));
	}

	strcpy(&data[12 * EKANS_NUM_SCORES], gEkansScoreName);

	int fd = creat(STORAGE_PATH "ekans", 0777);
	if(fd < 0)
		return;
	
	write(fd, data, EKANS_SAVEDATA_SIZE);
	close(fd);
}

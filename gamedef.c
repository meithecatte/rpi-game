#include "global.h"

game_t gGames[] = {
	{
		.gameIndex = GAME_EKANS,
		.initFunction = NULL,
		.startFunction = NULL,
		.renderFunction = NULL,
		.stopFunction = NULL,
		.cleanupFunction = NULL,
		.menuIconPath = "assets/snake_menu_logo.png"
	}
};

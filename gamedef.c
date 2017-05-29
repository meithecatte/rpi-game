#include "global.h"

game_t gGames[] = {
	{
		.gameIndex = GAME_EKANS,
		.initFunction = NULL,
		.startFunction = NULL,
		.renderFunction = NULL,
		.stopFunction = NULL,
		.cleanupFunction = NULL,
		.menuNameTop = "Ekans",
		.menuNameBottom = "",
		.menuIconPath = "assets/ekans/menu_logo.png"
	}
};

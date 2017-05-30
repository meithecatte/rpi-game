#include "global.h"
#include "ekans.h"

game_t gGames[] = {
	{
		.gameIndex = GAME_EKANS,
		.initFunction = NULL,
		.startFunction = Ekans_StartFunction,
		.renderFunction = Ekans_RenderFunction,
		.stopFunction = Ekans_StopFunction,
		.cleanupFunction = NULL,
		.menuNameTop = "Ekans",
		.menuNameBottom = "",
		.menuIconPath = "assets/ekans/menu_logo.png"
	}
};

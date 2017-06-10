#include "global.h"
#include "ekans/ekans.h"

game_t gGames[] = {
	{
		.gameIndex = GAME_EKANS,
		.startFunction = Ekans_StartFunc,
		.renderFunction = Ekans_RenderFunc,
		.menuNameTop = "Ekans",
		.menuNameBottom = "",
		.menuIconPath = "assets/ekans/menu_logo.png"
	}
};

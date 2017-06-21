#include "global.h"
#include "ekans/ekans.h"

game_t gGames[GAME_COUNT] = {
	[GAME_POWEROFF] = { // not really a game, just a menu entry
		.startFunction = PowerOff_StartFunc,
		.renderFunction = PowerOff_RenderFunc,
		.menuNameTop = "Power Off",
		.menuNameBottom = "",
		.menuIconPath = "assets/poweroff.png"
	},

	[GAME_EKANS] = {
		.startFunction = Ekans_StartFunc,
		.renderFunction = Ekans_RenderFunc,
		.menuNameTop = "Ekans",
		.menuNameBottom = "",
		.menuIconPath = "assets/ekans/menu_logo.png"
	}
};

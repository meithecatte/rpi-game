# rpi-game

~~Used
    ../configure --disable-pulseaudio --disable-esd --disable-video-mir --disable-video-wayland --disable-video-x11 --disable-video-opengl --disable-joystick --disable-haptic --enable static
for configuring SDL~~, also uses `SDL_image` and `SDL_mixer`

## Adding a new game

 * Add to `GAMES` in `Makefile`
 * Add to `game_index_t` in `gamedef.h`
 * Add to `gamedef.c`
 * Assets in `assets/gamename/*`
 * Code in `gamename/*`
 * `startFunc` is called once, then `renderFunc` every frame
 * to exit, cleanup everything and call `GameExit`

## Save format used by games

### Ekans

| Bytes     | Corresponding variable                            |
|:---------:| ------------------------------------------------- |
| 0 - 119   | `Ekans_ScoresTableEntry gEkansHighscores[10]`     |
| 120 - 127 | `char gEkansScoreName[8]`                         |

#### Ekans_ScoresTableEntry

| Bytes     | Corresponding variable                            |
|:---------:| ------------------------------------------------- |
| 0 - 7     | `char name[8]`                                    |
| 8 - 11    | `int score`                                       |

#ifndef GAME_MANAGER
#define GAME_MANAGER

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "SDL_set.h"
#include "characters.h"
#include "object.h"
#include "path_finder.h"
#include "s_map.h"
#include "score.h"
#include "portal.h"

#define WINDOW_W 800
#define WINDOW_H 600
#define FPS 60
#define SCORE_COORD_X 10
#define SCORE_COORD_Y 20
#define HP_COORD_X 100
#define HP_COORD_Y 10

void mx_set_icon(SDL_Window* window);
void mx_main_menu(SDL_Renderer *renderer, int hover);
void mx_drawImage(SDL_Texture *texture, SDL_Renderer *rend, int x, int y);
void mx_help(SDL_Renderer *renderer);
void mx_game_over(SDL_Renderer *renderer);
void mx_pause(SDL_Renderer *renderer);
void print_score(SDL_Renderer *rend, t_stepan *stepan, t_coin *coin);
void init(SDL_Renderer *rend, SDL_Window *win, t_map *map, t_coin *objects, t_powerup *watermelon, t_stepan *stepan, t_score *score, t_score *hp, Uint32 render_flags);

#endif

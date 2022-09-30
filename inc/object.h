#ifndef COINS_H
#define COINS_H

#include "characters.h"
#include "s_map.h"
#include "stdbool.h"

#define COINS_COST (10)
#define ORCS_COST (200)

typedef struct s_coin {
    SDL_Rect rect;
    SDL_Texture *text_coin;

    int block_w, block_h;
    int count_coin;
} t_coin;

typedef struct s_powerup {
    SDL_Texture *curentImage;
    SDL_Surface *surface;
    SDL_Rect Rect;
    SDL_Rect position;
    int block_w, block_h;
    int frameWidth, frameHeight;
    int textureW, textureH;
    int frameTime;
    bool check;
    int count_powerup;
} t_powerup;

t_coin *coin_make(t_map *map, SDL_Renderer *rend);
t_powerup *powerup_make(t_map *map, SDL_Renderer *rend);
bool check_powerup(t_map *map, t_stepan *stepan);
bool check_coin(t_map *map, t_stepan *stepan, t_coin *coin, bool *state_win);
void coin_render(t_coin *coin, SDL_Renderer *rend, t_map *map);
void powerup_render(t_powerup *watermelon, SDL_Renderer *rend, t_map *map);
void coin_free(t_coin *coin);
void powerup_free(t_powerup *powerup);

#endif


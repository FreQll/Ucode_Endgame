#ifndef PORTAL_H
#define PORTAL_H

#include "characters.h"
#include "s_map.h"
#include "stdbool.h"

typedef struct s_portal {
    SDL_Texture *curentImage;
    SDL_Surface *surface;
    SDL_Rect Rect;
    SDL_Rect position;
    int block_w, block_h;
    int frameWidth, frameHeight;
    int textureW, textureH;
    int frameTime;
    bool status;
    bool check;
    int x, y;
} t_portal;

t_portal *portal_make(t_map *map, SDL_Renderer *rend);
void portal_render(t_portal *portal, SDL_Renderer *rend, t_map *map);
void portal_free(t_portal *portal);
bool enter_to_portal(t_stepan *stepan, t_portal *portal);

#endif

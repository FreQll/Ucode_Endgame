#ifndef CHARACTERS_H
#define CHARACTERS_H

#include "SDL_set.h"
#include "s_map.h"
#include <stdbool.h>


#define S_SPEED 8
#define O_SPEED 10
#define DIFFICULTY 1

typedef struct s_stepan {
    int x;
    int y;
    int hp;
    float speed;

    int offset_y, offset_x;
    int direction;

    SDL_Texture *normal_status, *powerup_status;
    SDL_Rect rect, rect1;
    SDL_Rect position;
    int block_w, block_h;
    int frameWidth, frameHeight;
    int textureW, textureH;
    int textureW1, textureH1;
    int frameTime;
    int status;

    bool cooldown;

} t_stepan;

enum e_directions {
    D_STOP,
    D_UP,
    D_DOWN,
    D_RIGHT,
    D_LEFT
};

typedef struct s_orc {
    int x;
    int y;
    int p_x;
    int p_y;
    int mode;  // 0 - frigheting / 1 - chasing / 2 - to prison
    int num;
    SDL_Rect rect;
    SDL_Texture *text_f;
    SDL_Texture *text_c;
    SDL_Texture *text_p;
    float speed;
    int offset_y, offset_x;
    int block_w, block_h;
} t_orc;

t_stepan *stepan_make(t_map *map, SDL_Renderer *rend);
bool stepan_render(t_stepan *st, SDL_Renderer *rend, int time);
void stepan_free(t_stepan *stepan);

t_orc *orc_make(int num, t_map *map, SDL_Renderer *rend, int mode);
bool orc_render(t_orc *orc, SDL_Renderer *rend);
void orc_free(t_orc **orc);

#endif

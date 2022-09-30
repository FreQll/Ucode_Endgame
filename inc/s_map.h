#ifndef S_MAP
#define S_MAP

#include <stdbool.h>

#include "SDL_set.h"

typedef struct s_tile {
    bool wall;
    bool point;
    bool power_up;
    bool door;
} t_tile;

typedef struct s_rend_blok {
    bool is_configured;
    SDL_Rect dest;
    double angle;
    SDL_Texture *text;
} t_rend_blok;

typedef struct s_point {
    int x, y;
} t_point;

typedef struct s_map {
    int level_num;

    int lines, cols;  // y = line x = col

    int entry_y, entry_x;  // y = line x = col
    int exit_y, exit_x;    // y = line x = col

    t_tile **tiles;

    int offset_y, offset_x;
    int block_w, block_h;
    int texts_count;
    t_rend_blok back;
    SDL_Texture **texts;
    t_rend_blok **blocks;

    int tokens_left;

    int orks_left;
    t_point ork[10];

} t_map;

enum e_textures {
    BACKGROUND,
    WALL_BLOCK,
    WALL_LINE,
    WALL_T,
    WALL_X,
    WALL_CORNER,
    WALL_END,
    WALL_EMPY
};

t_map *map_make(int level_num,
                SDL_Renderer *rend,
                int win_w,
                int win_h);
void map_render(t_map *map, SDL_Renderer *rend);
void map_free(t_map *map);

#endif

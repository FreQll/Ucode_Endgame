#ifndef PATH_FINDER_H
#define PATH_FINDER_H
#include <math.h>
#include "s_map.h"
#include "characters.h"
#define PROBABILITY (2)
//void choose_dir(t_map * map, t_ork * ork);
int find_path(t_map * map, t_orc * ork, int s_y, int s_x, int dif);
void move_ork(t_orc * ork1, SDL_Renderer *rend, t_map * map, int div);
#endif

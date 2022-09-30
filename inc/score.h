#ifndef SCORE_H
#define SCORE_H

#include "../inc/game_manager.h"
#include "../inc/object.h"

typedef struct s_score
{
    char *score_count_text;
    SDL_Color color;
    TTF_Font *font;
    SDL_Surface *textSurface;
    SDL_Texture *tex;
    SDL_Rect textRect;

} t_score;

t_score *init_score();
void score_render(t_score *score, SDL_Renderer *rend, int count_coin);
t_score *init_hp(SDL_Renderer *rend);
void hp_render(t_score *hp, SDL_Renderer *rend, int i);
void score_free(t_score *score);

#endif

#include "../inc/game_manager.h"

void init(SDL_Renderer *rend, SDL_Window *win, t_map *map, t_coin *objects, t_powerup *watermelon, t_stepan *stepan, t_score *score, t_score *hp, Uint32 render_flags) {
    SDL_DestroyRenderer(rend);
    rend = SDL_CreateRenderer(win, -1, render_flags);
    if (!rend) {
        printf("error: %s\n", SDL_GetError());
        SDL_DestroyWindow(win);
        SDL_Quit();
        exit(-1);
    }
    map = map_make(1, rend, WINDOW_W, WINDOW_H);
    objects = coin_make(map, rend);
    watermelon = powerup_make(map, rend);
    stepan = stepan_make(map, rend);
    score = init_score();
    hp = init_hp(rend);
}

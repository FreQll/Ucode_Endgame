#include  "../inc/game_manager.h"

void mx_drawImage(SDL_Texture *texture, SDL_Renderer *rend, int x, int y) {
    SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
    SDL_RenderCopy(rend, texture, NULL, &dest);
    return;
}

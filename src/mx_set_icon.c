#include "../inc/game_manager.h"

void mx_set_icon(SDL_Window* window) {
    SDL_Surface *icon = IMG_Load("resource/images/icon.png");
    SDL_SetWindowIcon(window, icon);
    SDL_FreeSurface(icon);
}

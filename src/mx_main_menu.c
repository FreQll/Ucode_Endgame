#include "../inc/game_manager.h"


void mx_main_menu(SDL_Renderer *renderer, int hover) {
    //TTF_Font *font = TTF_OpenFont("resource/fonts/PressStart2P-Regular.ttf", 64);


    //SDL_Color Black = {rand() % (255 - 1 + 1) + 1, rand() % (255 - 1 + 1) + 1, rand() % (255 - 1 + 1) + 1, 255};

    //surfaces

    SDL_Surface *surface = IMG_Load("resource/images/menu/Menu.png");
    SDL_Surface *button_start = IMG_Load("resource/images/menu/1play.png");
    SDL_Surface *button_quit = IMG_Load("resource/images/menu/1quit.png");
    SDL_Surface *button_help = IMG_Load("resource/images/menu/1help.png");
    SDL_Surface *button_shop = IMG_Load("resource/images/menu/1shop.png");

    SDL_Surface *button_start_h = IMG_Load("resource/images/menu/2play.png");
    SDL_Surface *button_help_h = IMG_Load("resource/images/menu/2help.png");
    SDL_Surface *button_shop_h = IMG_Load("resource/images/menu/2shop.png");
    SDL_Surface *button_quit_h = IMG_Load("resource/images/menu/2quit.png");

    //textures

    SDL_Texture *menu_tex = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Texture *start_tex = SDL_CreateTextureFromSurface(renderer, button_start);
    SDL_Texture *quit_tex = SDL_CreateTextureFromSurface(renderer, button_quit);
    SDL_Texture *help_tex = SDL_CreateTextureFromSurface(renderer, button_help);
    SDL_Texture *shop_tex = SDL_CreateTextureFromSurface(renderer, button_shop);

    SDL_Texture *start_h_tex = SDL_CreateTextureFromSurface(renderer, button_start_h);
    SDL_Texture *help_h_tex = SDL_CreateTextureFromSurface(renderer, button_help_h);
    SDL_Texture *shop_h_tex = SDL_CreateTextureFromSurface(renderer, button_shop_h);
    SDL_Texture *quit_h_tex = SDL_CreateTextureFromSurface(renderer, button_quit_h);

    //SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surface);
    //SDL_Rect dest;
    //SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);

    //SDL_Surface *Title = TTF_RenderText_Solid(font, "BandMan", Black);
    //SDL_Texture *Title_tex = SDL_CreateTextureFromSurface(renderer, Title);

    //SDL_Rect Title_rect;

    //SDL_QueryTexture(Title_tex, NULL, NULL, &Title_rect.w, &Title_rect.h);
    //Title_rect.y = WINDOW_H / 6;
    //Title_rect.x = WINDOW_W / 4.5;


    SDL_RenderClear(renderer);

    mx_drawImage(menu_tex, renderer, 0, 0);
    
    //SDL_RenderCopy(renderer, tex, NULL, &dest);
    //SDL_RenderCopy(renderer, tex, NULL, &dest);

    switch (hover)
    {
    case 0:
        mx_drawImage(start_tex, renderer, -3, 50);
        mx_drawImage(help_tex, renderer, -3, 125);
        mx_drawImage(shop_tex, renderer, -3, 200);
        mx_drawImage(quit_tex, renderer, -3, 275);
        break;
    
    case 1:
        mx_drawImage(start_h_tex, renderer, -3, 50);
        mx_drawImage(help_tex, renderer, -3, 125);
        mx_drawImage(shop_tex, renderer, -3, 200);
        mx_drawImage(quit_tex, renderer, -3, 275);
        break;
    case 2:
        mx_drawImage(start_tex, renderer, -3, 50);
        mx_drawImage(help_h_tex, renderer, -3, 125);
        mx_drawImage(shop_tex, renderer, -3, 200);
        mx_drawImage(quit_tex, renderer, -3, 275);
        break;
    case 3:
        mx_drawImage(start_tex, renderer, -3, 50);
        mx_drawImage(help_tex, renderer, -3, 125);
        mx_drawImage(shop_h_tex, renderer, -3, 200);
        mx_drawImage(quit_tex, renderer, -3, 275);
        break;
    case 4: 
        mx_drawImage(start_tex, renderer, -3, 50);
        mx_drawImage(help_tex, renderer, -3, 125);
        mx_drawImage(shop_tex, renderer, -3, 200);
        mx_drawImage(quit_h_tex, renderer, -3, 275);
        break;
    }
    
    //SDL_RenderCopy(renderer, Title_tex, NULL, &Title_rect);

    SDL_RenderPresent(renderer);
    SDL_Delay(1000/6);
    // wait 1/60th of a second
    return;

}

void mx_help(SDL_Renderer *renderer) {
    SDL_Surface *help = IMG_Load("resource/images/menu/help.png");

    SDL_Texture *help_tex = SDL_CreateTextureFromSurface(renderer, help);

    mx_drawImage(help_tex, renderer, 0, 0);

    SDL_RenderPresent(renderer);
    SDL_Delay(1000/6);
    return;
}

void mx_pause(SDL_Renderer *renderer) {
    SDL_Surface *pause = IMG_Load("resource/images/menu/pause.png");

    SDL_Texture *pause_tex = SDL_CreateTextureFromSurface(renderer, pause);

    mx_drawImage(pause_tex, renderer, 0, 0);

    SDL_RenderPresent(renderer);
    SDL_Delay(1000/6);
    return;
}

void mx_game_over(SDL_Renderer *renderer) {
    SDL_Surface *pause = IMG_Load("resource/images/menu/game_over.png");

    SDL_Texture *pause_tex = SDL_CreateTextureFromSurface(renderer, pause);

    mx_drawImage(pause_tex, renderer, 0, 0);

    SDL_RenderPresent(renderer);
    SDL_Delay(1000/6);
    return;
}

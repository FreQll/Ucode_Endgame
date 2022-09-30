#include <stdio.h>

#include "../inc/game_manager.h"

int main(void) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER)) {
        printf("error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *win = SDL_CreateWindow("BandMan",
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       WINDOW_W,
                                       WINDOW_H,
                                       0);

    if (!win) {
        printf("error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    mx_set_icon(win);

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    Mix_Music *menu_music =
        Mix_LoadMUS("resource/sounds/music/menu.mp3");
    // Mix_Music *powerup = Mix_LoadMUS("resource/sounds/powerup.mp3");

    Uint32 render_flags =
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    SDL_Renderer *rend = SDL_CreateRenderer(win, -1, render_flags);
    if (!rend) {
        printf("error: %s\n", SDL_GetError());
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    if (TTF_Init() < 0) {
        printf("error: %s\n", SDL_GetError());
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }
    int map_level_num = 1;
    t_map *map = map_make(map_level_num, rend, WINDOW_W, WINDOW_H);
    if (!map) {
        getchar();
        return 0;
    }
    t_coin *objects = coin_make(map, rend);
    t_powerup *watermelon = powerup_make(map, rend);
    t_stepan *stepan = stepan_make(map, rend);
    t_score *score = init_score();
    t_score *hp = init_hp(rend);
    t_portal *portal = portal_make(map, rend);
    ///////////////////////////////
    t_orc *orcs[map->orks_left];
    for (int i = 0; i < map->orks_left; i++)
        orcs[i] = orc_make(i, map, rend, 0);
    ///////////////////////////////

    int up = 0, left = 0, down = 0, right = 0;
    int close = 0;

    int menu = 1;
    int help_menu = 0;
    int hover_button = 0;
    int mouseX, mouseY;
    int pause = 0;
    int once_music = 0;

    Mix_PlayMusic(menu_music, -1);

    unsigned long timer_old = SDL_GetTicks();
    unsigned long timer_new = SDL_GetTicks();

    unsigned long timer_powerup_beg = SDL_GetTicks();
    // unsigned long timer_powerup_end = SDL_GetTicks();
    /////
    int musicOn = 1;
    bool state_win = false;
    bool game_over = false;
    bool init = false;

    int temp_score = 0;
    int temp_hp;
    while (!close) {
        // process events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    close = 1;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.scancode) {
                        case SDL_SCANCODE_W:
                        case SDL_SCANCODE_UP:
                            up = 1;
                            down = 0;
                            left = 0;
                            right = 0;
                            break;
                        case SDL_SCANCODE_A:
                        case SDL_SCANCODE_LEFT:
                            left = 1;
                            right = 0;
                            up = 0;
                            down = 0;
                            break;
                        case SDL_SCANCODE_S:
                        case SDL_SCANCODE_DOWN:
                            down = 1;
                            up = 0;
                            left = 0;
                            right = 0;
                            break;
                        case SDL_SCANCODE_D:
                        case SDL_SCANCODE_RIGHT:
                            right = 1;
                            left = 0;
                            up = 0;
                            down = 0;
                            break;
                        case SDL_SCANCODE_M:
                            if (musicOn) {
                                Mix_VolumeMusic(0);
                                musicOn = 0;
                            }
                            else {
                                Mix_VolumeMusic(MIX_MAX_VOLUME);
                                musicOn = 1;
                            }
                            break;
                        case SDL_SCANCODE_R:
                            if (game_over) {
                                SDL_DestroyRenderer(rend);
                                rend = SDL_CreateRenderer(win,
                                                          -1,
                                                          render_flags);
                                if (!rend) {
                                    printf("error: %s\n",
                                           SDL_GetError());
                                    SDL_DestroyWindow(win);
                                    SDL_Quit();
                                    exit(-1);
                                }
                                map_level_num = 1;
                                map = map_make(1,
                                               rend,
                                               WINDOW_W,
                                               WINDOW_H);
                                objects = coin_make(map, rend);
                                watermelon = powerup_make(map, rend);
                                stepan = stepan_make(map, rend);
                                score = init_score();
                                hp = init_hp(rend);
                                portal = portal_make(map, rend);
                                state_win = false;
                                game_over = false;
                                up = 0;
                                left = 0;
                                down = 0;
                                right = 0;
                                for (int i = 0; i < map->orks_left;
                                     i++) {
                                    orc_free(&(orcs[i]));
                                }
                                for (int i = 0; i < map->orks_left;
                                     i++) {
                                    orcs[i] = orc_make(i, map, rend, 0);
                                }
                                /*ork1 = orc_make(0, map, rend, 0);
                                 ork2 = orc_make(1, map, rend, 0);
                                 ork3 = orc_make(2, map, rend, 0);
                                 orcs[0] = *ork1;
                                 orcs[1] = *ork2;
                                 orcs[2] = *ork3;*/
                                state_win = false;
                                menu = 1;
                                once_music = 0;
                            }
                            break;
                        case SDL_SCANCODE_ESCAPE:
                            if (!menu && !help_menu && pause == 0) {
                                pause = 1;
                                Mix_PauseMusic();
                            }
                            else if (pause) {
                                pause = 0;
                                Mix_ResumeMusic();
                            }
                            break;
                        case SDL_SCANCODE_RETURN:
                            if (help_menu == 1) help_menu = 0;
                            break;
                    }
                    break;
                case SDL_KEYUP:
                    switch (event.key.keysym.scancode) {
                        case SDL_SCANCODE_W:
                        case SDL_SCANCODE_UP:
                            down = 0;
                            left = 0;
                            right = 0;
                            break;
                        case SDL_SCANCODE_A:
                        case SDL_SCANCODE_LEFT:
                            right = 0;
                            up = 0;
                            down = 0;
                            break;
                        case SDL_SCANCODE_S:
                        case SDL_SCANCODE_DOWN:
                            up = 0;
                            left = 0;
                            right = 0;
                            break;
                        case SDL_SCANCODE_D:
                        case SDL_SCANCODE_RIGHT:
                            left = 0;
                            up = 0;
                            down = 0;
                            break;
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (menu) {
                        if ((mouseX >= 90 && mouseX <= 290) &&
                            (mouseY >= 190 && mouseY <= 235)) {
                            menu = 0;
                            Mix_HaltMusic();
                        }
                        if ((mouseX >= 90 && mouseX <= 290) &&
                            (mouseY >= 265 && mouseY <= 315)) {
                            help_menu = 1;
                        }
                        if ((mouseX >= 90 && mouseX <= 290) &&
                            (mouseY >= 410 && mouseY <= 465)) {
                            close = 1;
                        }
                    }
                    break;
            }
        }

        if (help_menu) {
            mx_help(rend);
        }
        else if (game_over) {
            mx_game_over(rend);
        }
        else if (pause) {
            mx_pause(rend);
        }
        else if (menu) {
            if (!once_music) {
                Mix_PlayMusic(menu_music, -1);
                once_music = 1;
            }
            hover_button = 0;
            if ((mouseX >= 90 && mouseX <= 290) &&
                (mouseY >= 190 && mouseY <= 235)) {
                hover_button = 1;
            }
            if ((mouseX >= 90 && mouseX <= 290) &&
                (mouseY >= 265 && mouseY <= 315)) {
                hover_button = 2;
            }
            if ((mouseX >= 90 && mouseX <= 290) &&
                (mouseY >= 410 && mouseY <= 465)) {
                hover_button = 4;
            }
            SDL_GetMouseState(&mouseX, &mouseY);
            // printf("%d\t%d\n", mouseX, mouseY);
            mx_main_menu(rend, hover_button);
        }
        else {
            if (init) {
                SDL_DestroyRenderer(rend);
                rend = SDL_CreateRenderer(win, -1, render_flags);
                if (!rend) {
                    printf("error: %s\n", SDL_GetError());
                    SDL_DestroyWindow(win);
                    SDL_Quit();
                    exit(-1);
                }
                map = map_make(1, rend, WINDOW_W, WINDOW_H);
                map_level_num += 1;
                objects = coin_make(map, rend);
                watermelon = powerup_make(map, rend);
                stepan = stepan_make(map, rend);
                score = init_score();
                hp = init_hp(rend);
                portal = portal_make(map, rend);
                up = 0;
                left = 0;
                down = 0;
                right = 0;

                for (int i = 0; i < map->orks_left; i++) {
                    orc_free(&(orcs[i]));
                    orcs[i] = orc_make(i, map, rend, 0);
                    orcs[i]->speed = O_SPEED + map_level_num;
                }
                /*ork1 = orc_make(0, map, rend, 0);
                ork2 = orc_make(1, map, rend, 0);
                ork3 = orc_make(2, map, rend, 0);
                orcs[0] = *ork1;
                orcs[1] = *ork2;
                orcs[2] = *ork3;*/

                if (state_win) {
                    objects->count_coin = temp_score;
                    stepan->hp = temp_hp;
                }
                game_over = false;
                state_win = false;
                init = false;
            }

            timer_old = timer_new;
            timer_new = SDL_GetTicks();

            SDL_RenderClear(rend);
            map_render(map, rend);
            coin_render(objects, rend, map);
            powerup_render(watermelon, rend, map);
            portal_render(portal, rend, map);

            score_render(score, rend, objects->count_coin);

            if (stepan->hp == 0) menu = 1;

            if (stepan_render(stepan, rend, timer_new - timer_old)) {
                check_coin(map, stepan, objects, &portal->status);
                if (portal->status && enter_to_portal(stepan, portal)) {
                    portal_free(portal);
                    init = true;
                    state_win = true;
                    Mix_PauseMusic();
                }

                if (check_powerup(map, stepan)) {
                    stepan->status = 2;
                    timer_powerup_beg = SDL_GetTicks();
                    for (int i = 0; i < map->orks_left; i++)
                        orcs[i]->mode = 1;
                }
                if (stepan->status == 2)
                    if (10000 < SDL_GetTicks() - timer_powerup_beg) {
                        stepan->status = 1;
                        for (int i = 0; i < map->orks_left; i++) {
                            // orcs[i]->speed = O_SPEED;
                            orcs[i]->mode = 0;
                        }
                    }
                if (up && !down &&
                    !map->tiles[stepan->y - 1][stepan->x].wall &&
                    !map->tiles[stepan->y - 1][stepan->x].door) {
                    stepan->y--;
                    stepan->direction = D_UP;
                }
                else if (down && !up &&
                         !map->tiles[stepan->y + 1][stepan->x].wall &&
                         !map->tiles[stepan->y + 1][stepan->x].door) {
                    stepan->y++;
                    stepan->direction = D_DOWN;
                }
                else if (left && !right &&
                         !map->tiles[stepan->y][stepan->x - 1].wall &&
                         !map->tiles[stepan->y][stepan->x - 1].door) {
                    stepan->x--;
                    stepan->direction = D_LEFT;
                }
                else if (right && !left &&
                         !map->tiles[stepan->y][stepan->x + 1].wall &&
                         !map->tiles[stepan->y][stepan->x + 1].door) {
                    stepan->x++;
                    stepan->direction = D_RIGHT;
                }
                else {
                    if (stepan->direction == D_UP &&
                        !map->tiles[stepan->y - 1][stepan->x].wall &&
                        !map->tiles[stepan->y - 1][stepan->x].door) {
                        stepan->y--;
                    }
                    else if (stepan->direction == D_DOWN &&
                             !map->tiles[stepan->y + 1][stepan->x]
                                  .wall &&
                             !map->tiles[stepan->y + 1][stepan->x]
                                  .door) {
                        stepan->y++;
                    }
                    else if (stepan->direction == D_LEFT &&
                             !map->tiles[stepan->y][stepan->x - 1]
                                  .wall &&
                             !map->tiles[stepan->y][stepan->x - 1]
                                  .door) {
                        stepan->x--;
                    }
                    else if (stepan->direction == D_RIGHT &&
                             !map->tiles[stepan->y][stepan->x + 1]
                                  .wall &&
                             !map->tiles[stepan->y][stepan->x + 1]
                                  .door) {
                        stepan->x++;
                    }
                }
            }
            bool touch = false;
            for (int i = 0; i < map->orks_left; i++) {
                /*int coord_y = stepan->y * stepan->frameWidth +
                stepan->offset_y; int coord_x = stepan->x *
                stepan->frameHeight + stepan->offset_x;*/
                if (orcs[i]->mode == 2 && orcs[i]->x == map->ork[i].x &&
                    orcs[i]->y == map->ork[i].y) {
                    orcs[i]->speed = O_SPEED + map_level_num;
                    orcs[i]->mode = 1;
                }
                if (orcs[i]->speed > 4 * stepan->speed &&
                    orcs[i]->mode != 2) {
                    orcs[i]->speed = O_SPEED;
                }
                if ((stepan->position.y >=
                         orcs[i]->rect.y - orcs[i]->rect.h / 2 &&
                     stepan->position.y <=
                         orcs[i]->rect.y + orcs[i]->rect.h / 2) &&
                    (stepan->position.x >=
                         orcs[i]->rect.x - orcs[i]->rect.w / 2 &&
                     stepan->position.x <=
                         orcs[i]->rect.x + orcs[i]->rect.w / 2)) {
                    touch = true;
                    if (stepan->status == 2 && orcs[i]->mode != 2) {
                        objects->count_coin += ORCS_COST;
                    }
                    if (orcs[i]->mode != 2 && stepan->status == 1) {
                        if (stepan->cooldown == false) {
                            stepan->cooldown = true;
                            stepan->hp--;
                        }
                    }
                    else {
                        // orcs[i]->speed *= 10;
                        orcs[i]->mode = 2;
                    }
                    if (stepan->hp == 0) {
                        menu = 1;
                        init = true;
                        state_win = false;
                        game_over = true;
                    }
                }
                move_ork(orcs[i],
                         rend,
                         map,
                         find_path(map,
                                   orcs[i],
                                   stepan->y,
                                   stepan->x,
                                   map_level_num));
            }
            if (!touch) {
                stepan->cooldown = false;
            }
            temp_score = objects->count_coin;
            temp_hp = stepan->hp;

            for (int i = 0; i < stepan->hp; i++)
                hp_render(hp, rend, i);

            SDL_RenderPresent(rend);
            // wait 1/60th of a second
            SDL_Delay(1000 / 60);
        }
    }

    stepan_free(stepan);
    for (int i = 0; i < map->orks_left; i++) {
        orc_free(&orcs[i]);
    }
    map_free(map);
    coin_free(objects);
    powerup_free(watermelon);
    score_free(score);
    score_free(hp);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
}

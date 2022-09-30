#include "../inc/object.h"

t_coin *coin_make(t_map *map, SDL_Renderer *rend) {
    t_coin *object = malloc(sizeof(t_map));
    object->rect.h = map->block_h;
    object->rect.w = map->block_w;
    object->rect.x = map->offset_x + map->block_w * map->entry_x;
    object->rect.y = map->offset_y + map->block_h * map->entry_y;
    object->block_h = map->block_h;
    object->block_w = map->block_w;
    object->count_coin = 0;

    SDL_Surface *surface = IMG_Load("resource/images/coin.png");
    SDL_Texture *tex = SDL_CreateTextureFromSurface(rend, surface);
    object->text_coin = malloc(sizeof(SDL_Texture *));
    object->text_coin = tex;

    return object;
}

t_powerup *powerup_make(t_map *map, SDL_Renderer *rend) {
    t_powerup *watermelon = malloc(sizeof(t_map));
    watermelon->curentImage = NULL;

    watermelon->surface = IMG_Load("resource/images/watermelon.png");
    watermelon->curentImage = SDL_CreateTextureFromSurface(rend, watermelon->surface);
    SDL_QueryTexture(watermelon->curentImage, NULL, NULL, &watermelon->textureW, &watermelon->textureH);
    watermelon->position.x = 0;
    watermelon->position.y = 0;
    watermelon->position.w = map->block_w * 2;
    watermelon->position.h = map->block_h * 2;
    watermelon->frameWidth = watermelon->textureW / 7;
    watermelon->frameHeight = watermelon->textureH;
    watermelon->Rect.x = 0;
    watermelon->Rect.y = 0;
    watermelon->Rect.w = watermelon->frameWidth;
    watermelon->Rect.h = watermelon->frameHeight;
    watermelon->check = true;
    watermelon->frameTime = 0;
    map->block_h = map->block_h;
    return watermelon;
}

bool check_powerup(t_map *map, t_stepan *stepan) {
    if (map->tiles[stepan->y][stepan->x].power_up) {
        Mix_Music *powerup = Mix_LoadMUS("resource/sounds/powerup.mp3");
        Mix_PlayMusic(powerup, 1);

        map->tiles[stepan->y][stepan->x].power_up = false;
        return true;
    }
    return false;
}

bool check_coin(t_map *map, t_stepan *stepan, t_coin *coin, bool *state_win) {
    if (map->tiles[stepan->y][stepan->x].point) {
        map->tiles[stepan->y][stepan->x].point = false;
        coin->count_coin += COINS_COST;
        map->tokens_left--;
        if (map->tokens_left == 0)
            *state_win = true;
        return true;
    }
    return false;
}

void coin_render(t_coin *coin, SDL_Renderer *rend, t_map *map) {
    for (int i = 0; i < map->lines; i++) {
        for (int j = 0; j < map->cols; j++) {
            if (map->tiles[i][j].point) {
                coin->rect.x = j * map->block_w + map->offset_x;
                coin->rect.y = i * map->block_h + map->offset_y;
                SDL_RenderCopy(rend, coin->text_coin, NULL, &coin->rect);
            }
        }
    }
}

void powerup_render(t_powerup *watermelon, SDL_Renderer *rend, t_map *map) {
    int diff_x = watermelon->position.h - map->block_h;
    int diff_y = watermelon->position.w - map->block_w;

    for (int i = 0; i < map->lines; i++) {
        for (int j = 0; j < map->cols; j++) {
            if (map->tiles[i][j].power_up) {
                watermelon->position.x = j * map->block_w + map->offset_x - diff_x / 2 - 3;
                watermelon->position.y = i * map->block_h + map->offset_y - diff_y / 2;
                watermelon->frameTime++;
                if (60 / watermelon->frameTime == 7 && watermelon->check) {
                    if (watermelon->Rect.x == watermelon->frameWidth * 5) {
                        watermelon->Rect.x += watermelon->frameWidth;
                        watermelon->check = false;
                    }
                    if (watermelon->check) {
                        watermelon->frameTime = 0;
                        watermelon->Rect.x += watermelon->frameWidth;
                    }
                }
                SDL_RenderCopy(rend, watermelon->curentImage, &watermelon->Rect, &watermelon->position);
            }
        }
    }
}

void coin_free(t_coin *coin) {
    if (coin->text_coin) {
        SDL_DestroyTexture(coin->text_coin);
        coin->text_coin = NULL;
    }
    free(coin);
}

void powerup_free(t_powerup *powerup) {
    if (powerup->curentImage) {
        SDL_DestroyTexture(powerup->curentImage);
        powerup->curentImage = NULL;
    }
    free(powerup);
}


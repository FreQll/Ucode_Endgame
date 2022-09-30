#include "../inc/characters.h"

t_stepan *stepan_make(t_map *map, SDL_Renderer *rend) {
    t_stepan *stepan = malloc(sizeof(t_stepan));
    stepan->normal_status = NULL;

    SDL_Surface *surface = IMG_Load("resource/images/stepan2.png");

    stepan->normal_status = SDL_CreateTextureFromSurface(rend, surface);
    SDL_QueryTexture(stepan->normal_status, NULL, NULL, &stepan->textureW, &stepan->textureH);
    stepan->position.x = map->offset_x + map->block_w + map->block_w * map->entry_x;
    stepan->position.y = map->offset_y + map->block_h * map->entry_y;
    stepan->position.w = map->block_w;
    stepan->position.h = map->block_h;
    stepan->x = map->entry_x;
    stepan->y = map->entry_y;
    stepan->frameWidth = stepan->textureW / 3;
    stepan->frameHeight = stepan->textureH;
    stepan->rect.x = 0;
    stepan->rect.y = 0;
    stepan->rect.w = stepan->frameWidth;
    stepan->rect.h = stepan->frameHeight;
    stepan->frameTime = 0;
    stepan->speed = S_SPEED;
    stepan->direction = D_STOP;
    stepan->hp = 3;
    stepan->offset_x = map->offset_x;
    stepan->offset_y = map->offset_y;
    stepan->block_h = map->block_h;
    stepan->block_w = map->block_w;

    surface = IMG_Load("resource/images/evil_stepan.png");
    stepan->powerup_status = SDL_CreateTextureFromSurface(rend, surface);
    stepan->status = 1;

    stepan->cooldown = false;
    return stepan;
}

static void mini_render(t_stepan *stepan, SDL_Renderer *rend, bool status) {
    SDL_Texture *tex_1 = NULL;
    SDL_Rect tex_pos = stepan->position;
    // SDL_Rect rect;

    if (stepan->status == 1) {
        stepan->frameTime++;
        if (60 / stepan->frameTime == 3) {
            stepan->frameTime = 0;
            stepan->rect.x += stepan->frameWidth;
            if (stepan->rect.x == stepan->frameWidth * 3) {
                stepan->rect.x = 0;
            }
            if (!status) {
                stepan->rect.x = 0;
            }
        }
        tex_1 = stepan->normal_status;
        tex_pos = stepan->position;
        tex_pos.w *= 2;
        tex_pos.h *= 2;
        tex_pos.x -= 12;
        tex_pos.y -= 12;
        if (stepan->direction == D_UP) {
            SDL_RenderCopyEx(rend, tex_1, &stepan->rect, &tex_pos, -90, NULL, SDL_FLIP_NONE);
        } else if (stepan->direction == D_DOWN) {
            SDL_RenderCopyEx(rend, tex_1, &stepan->rect, &tex_pos, 90, NULL, SDL_FLIP_NONE);
        } else if (stepan->direction == D_LEFT) {
            SDL_RenderCopyEx(rend, tex_1, &stepan->rect, &tex_pos, 0, NULL, SDL_FLIP_HORIZONTAL);
        } else {
            SDL_RenderCopy(rend, tex_1, &stepan->rect, &tex_pos);
        }
    }
    if (stepan->status == 2) {
        tex_1 = stepan->powerup_status;
        tex_pos = stepan->position;
        tex_pos.w *= 2;
        tex_pos.h *= 2;
        tex_pos.x -= 12;
        tex_pos.y -= 12;
        if (stepan->direction == D_UP) {
            SDL_RenderCopyEx(rend, tex_1, NULL, &tex_pos, -90, NULL, SDL_FLIP_NONE);
        }
        else if (stepan->direction == D_DOWN) {
            SDL_RenderCopyEx(rend, tex_1, NULL, &tex_pos, 90, NULL, SDL_FLIP_NONE);
        }
        else if (stepan->direction == D_LEFT) {
            SDL_RenderCopyEx(rend, tex_1, NULL, &tex_pos, 0, NULL, SDL_FLIP_HORIZONTAL);
        }
        else {
            SDL_RenderCopy(rend, tex_1, NULL, &tex_pos);
        }
    }

    /*if (stepan->direction == D_UP) {
        SDL_RenderCopyEx(rend, tex_1, &rect, &tex_pos, -90, NULL, SDL_FLIP_NONE);
    }
    else if (stepan->direction == D_DOWN) {
        SDL_RenderCopyEx(rend, tex_1, &rect, &tex_pos, 90, NULL, SDL_FLIP_NONE);
    }
    else if (stepan->direction == D_LEFT) {
        SDL_RenderCopyEx(rend, tex_1, &rect, &tex_pos, 0, NULL, SDL_FLIP_HORIZONTAL);
    }
    else {
        SDL_RenderCopy(rend, tex_1, &rect, &tex_pos);
    }*/
    
}

bool stepan_render(t_stepan *st, SDL_Renderer *rend, int time) { // true if stepan in the point
    if (st->position.x > st->x * st->block_w + st->offset_x) {
        st->position.x -= st->block_w * st->speed * time / 1000;
        if (st->position.x < st->x * st->block_w + st->offset_x) {
            st->position.x = st->x * st->block_w + st->offset_x;
        }
    }
    else if (st->position.x < st->x * st->block_w + st->offset_x) {
        st->position.x += st->block_w * st->speed * time / 1000;
        if (st->position.x > st->x * st->block_w + st->offset_x) {
            st->position.x = st->x * st->block_w + st->offset_x;
        }
    }
    else if (st->position.y > st->y * st->block_h + st->offset_y) {
        st->position.y -= st->block_h * st->speed * time / 1000;
        if (st->position.y < st->y * st->block_h + st->offset_y) {
            st->position.y = st->y * st->block_h + st->offset_y;
        }
    }
    else if (st->position.y < st->y * st->block_h + st->offset_y) {
        st->position.y += st->block_h * st->speed * time / 1000;
        if (st->position.y > st->y * st->block_h + st->offset_y) {
            st->position.y = st->y * st->block_h + st->offset_y;
        }
    }
    else {
        mini_render(st, rend, false);
        return true;
    }
    if (st->position.y == st->y * st->block_h + st->offset_y
        && st->position.x == st->x * st->block_w + st->offset_x) {
        mini_render(st, rend, false);
        return true;
    }
    mini_render(st, rend, true);
    return false;
}

void stepan_free(t_stepan *stepan) {
    if (stepan->normal_status) {
        SDL_DestroyTexture(stepan->normal_status);
        stepan->normal_status = NULL;
    }
    free(stepan);
}


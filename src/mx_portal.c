#include "../inc/portal.h"

t_portal *portal_make(t_map *map, SDL_Renderer *rend) {
    t_portal *portal = malloc(sizeof(t_map));

    portal->curentImage = NULL;
    portal->surface = IMG_Load("resource/images/portal-1.png");
    portal->curentImage = SDL_CreateTextureFromSurface(rend, portal->surface);
    SDL_QueryTexture(portal->curentImage, NULL, NULL, &portal->textureW, &portal->textureH);
    portal->x = map->exit_x;
    portal->y = map->exit_y;
    portal->position.x = map->offset_x + map->block_w * (map->exit_x - 1.1);
    portal->position.y = map->offset_y + map->block_h * (map->exit_y - 2);
    portal->position.w = map->block_w * 4;
    portal->position.h = map->block_h * 4;
    portal->frameWidth = portal->textureW / 8;
    portal->frameHeight = portal->textureH;
    portal->Rect.x = 0;
    portal->Rect.y = 0;
    portal->Rect.w = portal->frameWidth;
    portal->Rect.h = portal->frameHeight;
    portal->status = false;
    portal->check = true;
    portal->frameTime = 0;
    map->block_h = map->block_h;

    return portal;   
}

void portal_render(t_portal *portal, SDL_Renderer *rend, t_map *map) {

    if (portal->status) {
        portal->frameTime++;
        if (60 / portal->frameTime == 8 && portal->check) {
            if (portal->Rect.x == portal->frameWidth * 6) {
                portal->Rect.x += portal->frameWidth;
                portal->check = false;
            }
            if (portal->check) {
                portal->frameTime = 0;
                portal->Rect.x += portal->frameWidth;
            }
        }
        SDL_RenderCopy(rend, portal->curentImage, &portal->Rect, &portal->position);
        map->block_h = map->block_h;
    }
}

bool enter_to_portal(t_stepan *stepan, t_portal *portal) {
    if (stepan->x == portal->x && stepan->y == portal->y)
        return true;
    return false;
}

void portal_free(t_portal *portal) {
    if (portal->curentImage) {
        SDL_DestroyTexture(portal->curentImage);
        portal->curentImage = NULL;
    }
    free(portal);
}


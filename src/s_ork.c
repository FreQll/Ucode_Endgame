#include "../inc/characters.h"

t_orc *orc_make(int num, t_map *map, SDL_Renderer *rend, int mode)
{
    t_orc *ork = malloc(sizeof(t_orc));
    ork->rect.h = map->block_h * 2;
    ork->rect.w = map->block_w * 2;

    int diff_x = ork->rect.h - map->block_h;
    int diff_y = ork->rect.w - map->block_w;

    ork->rect.x =
        map->offset_x + map->block_w * map->ork[num].x - diff_x / 2;
    ork->rect.y =
        map->offset_y + map->block_h * map->ork[num].y - diff_y / 2;

    ork->mode = mode;
    ork->num = num;
    ork->x = map->ork[num].x;
    ork->y = map->ork[num].y;
    ork->p_x = map->ork[num].x + 1;
    ork->p_y = map->ork[num].y - 1;
    switch (num)
    {
    case 0:
        ork->text_f = SDL_CreateTextureFromSurface(
            rend,
            IMG_Load("resource/images/orc_g_f.png"));
        ork->text_c = SDL_CreateTextureFromSurface(
            rend,
            IMG_Load("resource/images/orc_g_c.png"));
        ork->text_p = SDL_CreateTextureFromSurface(
            rend,
            IMG_Load("resource/images/orc_d.png"));
        break;
    case 1:
        ork->text_f = SDL_CreateTextureFromSurface(
            rend,
            IMG_Load("resource/images/orc_b_f.png"));
        ork->text_c = SDL_CreateTextureFromSurface(
            rend,
            IMG_Load("resource/images/orc_b_c.png"));
        ork->text_p = SDL_CreateTextureFromSurface(
            rend,
            IMG_Load("resource/images/orc_d.png"));
        break;
    case 2:
        ork->text_f = SDL_CreateTextureFromSurface(
            rend,
            IMG_Load("resource/images/orc_r_f.png"));
        ork->text_c = SDL_CreateTextureFromSurface(
            rend,
            IMG_Load("resource/images/orc_r_c.png"));
        ork->text_p = SDL_CreateTextureFromSurface(
            rend,
            IMG_Load("resource/images/orc_d.png"));
        break;
    default:
        break;
    }
    ork->block_h = map->block_h;
    ork->block_w = map->block_w;
    ork->offset_x = map->offset_x - diff_x / 2;
    ork->offset_y = map->offset_y - diff_y / 2;
    ork->speed = O_SPEED;
    return ork;
}

void rend_diff_tex(t_orc *orc, SDL_Renderer *rend)
{
    if (orc->mode == 0)
        SDL_RenderCopy(rend, orc->text_f, NULL, &orc->rect);
    else if (orc->mode == 1)
        SDL_RenderCopy(rend, orc->text_c, NULL, &orc->rect);
    else if (orc->mode == 2)
        SDL_RenderCopy(rend, orc->text_p, NULL, &orc->rect);
}

bool orc_render(t_orc *orc, SDL_Renderer *rend)
{
    if (orc->rect.x > orc->x * orc->block_w + orc->offset_x)
    {
        orc->rect.x -= orc->block_w * orc->speed / 100;
        if (orc->rect.x < orc->x * orc->block_w + orc->offset_x)
        {
            orc->rect.x = orc->x * orc->block_w + orc->offset_x;
        }
    }
    else if (orc->rect.x < orc->x * orc->block_w + orc->offset_x)
    {
        orc->rect.x += orc->block_w * orc->speed / 100;
        if (orc->rect.x > orc->x * orc->block_w + orc->offset_x)
        {
            orc->rect.x = orc->x * orc->block_w + orc->offset_x;
        }
    }
    else if (orc->rect.y > orc->y * orc->block_h + orc->offset_y)
    {
        orc->rect.y -= orc->block_h * orc->speed / 100;
        if (orc->rect.y < orc->y * orc->block_h + orc->offset_y)
        {
            orc->rect.y = orc->y * orc->block_h + orc->offset_y;
        }
    }
    else if (orc->rect.y < orc->y * orc->block_h + orc->offset_y)
    {
        orc->rect.y += orc->block_h * orc->speed / 100;
        if (orc->rect.y > orc->y * orc->block_h + orc->offset_y)
        {
            orc->rect.y = orc->y * orc->block_h + orc->offset_y;
        }
    }
    else
    {
        rend_diff_tex(orc, rend);
        return true;
    }
    if (orc->rect.y == orc->y * orc->block_h + orc->offset_y && orc->rect.x == orc->x * orc->block_w + orc->offset_x)
    {
        rend_diff_tex(orc, rend);
        return true;
    }
    rend_diff_tex(orc, rend);
    return false;
}

void orc_free(t_orc **ork) {
    if (*ork) {
        if ((*ork)->text_f)
        {
            SDL_DestroyTexture((*ork)->text_f);
            (*ork)->text_f = NULL;
        }
        if ((*ork)->text_c)
        {
            SDL_DestroyTexture((*ork)->text_c);
            (*ork)->text_c = NULL;
        }
        if ((*ork)->text_p)
        {
            SDL_DestroyTexture((*ork)->text_p);
            (*ork)->text_p = NULL;
        }
        (*ork) = NULL;
        free(*ork);
    }
}

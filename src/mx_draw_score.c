#include "../inc/game_manager.h"

char* toString(int n) {
    char *c;
    c = (char *)malloc(10 * sizeof(char));
    int v = 0; 
    while (n > 9) {
        c[v++] = (n % 10) + '0';
        n = n / 10;
    }
    c[v++] = n + '0';
    c[v] = '\0';
    char t;
    for (int i = 0; i < v / 2; i++) {
        t = c[i];
        c[i] = c[v - 1 - i];
        c[v - 1 - i] = t;
    }
    return c;
}

t_score *init_score() {
    t_score *score = malloc(sizeof(t_score));
    SDL_Color col = {255, 255, 255, 255};
    score->color = col;
    score->font = TTF_OpenFont("resource/fonts/PressStart2P-Regular.ttf", 20);
    score->textRect.x = score->textRect.y = 0;
    return score;
}

void score_render(t_score *score, SDL_Renderer *rend, int count_coin) {
    score->score_count_text = toString(count_coin);
    score->textSurface = TTF_RenderText_Solid(score->font, score->score_count_text, score->color);
    score->tex = SDL_CreateTextureFromSurface(rend, score->textSurface);
    SDL_QueryTexture(score->tex, NULL, NULL, &score->textRect.w, &score->textRect.h);
    score->textRect.x = SCORE_COORD_X;
    score->textRect.y = SCORE_COORD_Y;
    SDL_RenderCopy(rend, score->tex, NULL, &score->textRect);
    free(score->score_count_text);
}

t_score *init_hp(SDL_Renderer *rend) {
    t_score *hp = malloc(sizeof(t_score));
    SDL_Surface *surface = IMG_Load("resource/images/hp.png");
    SDL_Texture *tex = SDL_CreateTextureFromSurface(rend, surface);
    hp->tex = malloc(sizeof(SDL_Texture *));
    hp->tex = tex;
    return hp;
}

void hp_render(t_score *hp, SDL_Renderer *rend, int i) {
    SDL_QueryTexture(hp->tex, NULL, NULL, &hp->textRect.w, &hp->textRect.h);
    hp->textRect.x = 600 + HP_COORD_X;
    hp->textRect.y = i * 50;
    hp->textRect.w /= 12;
    hp->textRect.h /= 12;
    SDL_RenderCopy(rend, hp->tex, NULL, &hp->textRect);
}

void score_free(t_score *score) {
    if (score->tex) {
        SDL_DestroyTexture(score->tex);
        score->tex = NULL;
    }
    free(score);
}

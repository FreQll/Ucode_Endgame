#include "../inc/s_map.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static char *mx_strdup(const char *str) {
    int lenth = 0;
    char *string_copy;
    char *dst;
    const char *src = str;

    while (*(src++)) {
        lenth++;
    }

    string_copy = malloc(sizeof(char) * (lenth + 1));

    if (!string_copy) return NULL;
    string_copy[lenth] = '\0';

    dst = string_copy;
    src = str;

    while (*src) {
        *dst = *src;
        dst++;
        src++;
    }
    *dst = '\0';
    return string_copy;
}

// MAP_MAKE
static int init_values(t_map *new_map);
static int init_tiles(t_map *new_map);
static int init_texts(t_map *new_map, SDL_Renderer *renderer);
static int init_rend_blocks(t_map *new_map, int win_w, int win_h);

t_map *map_make(int level_num,
                SDL_Renderer *rend,
                int win_w,
                int win_h) {
    t_map *new_map = malloc(sizeof(t_map));
    new_map->level_num = level_num;
    new_map->texts = NULL;
    new_map->blocks = NULL;
    new_map->tiles = NULL;

    if (!(init_values(new_map))) {
        printf("Init values error\n");
        map_free(new_map);
        free(new_map);
        return NULL;
    }
    if (!(init_tiles(new_map))) {
        printf("Init tile error\n");
        map_free(new_map);
        free(new_map);
        return NULL;
    }
    if (!(init_texts(new_map, rend))) {
        printf("Init textures error\n");
        map_free(new_map);
        free(new_map);
        return NULL;
    }
    if (!(init_rend_blocks(new_map, win_w, win_h))) {
        printf("Init rend bloks error\n");
        map_free(new_map);
        free(new_map);
        return NULL;
    }
    return new_map;
}

// MAP_MAKE VALUES
static char *get_first_line(const char *file_path) {
    int file = open(file_path, O_RDONLY);

    if (file < 0) return NULL;

    char *info_line = malloc(21);
    char *info_element = info_line;
    char c;

    while (read(file, &c, 1) && c != '\n') {
        *info_element = c;
        info_element++;
    }

    *info_element = '\0';
    close(file);
    return info_line;
}

static int mx_atoi(const char *str) {
    int value = 0;
    while (*str && *str >= '0' && *str <= '9') {
        value *= 10;
        value += *str - '0';
        str++;
    }
    return value;
}

static const char *get_word(const char *str, int index) {
    bool is_word = true;
    while (*str) {
        if (is_word) {
            // skip spaces before word
            while (*str == ' ') {
                str++;
            }
            if (index == 0) {
                return str;
            }
            else {
                index--;
                is_word = false;
            }
        }
        else {
            while (*str != ' ') {
                str++;
            }
            is_word = true;
        }
    }
    return str;
}

static int init_values(t_map *new_map) {
    char *path = mx_strdup("resource/maps/level*/blueprint.txt");

    path[19] = new_map->level_num + '0';

    char *info_line = get_first_line(path);

    free(path);
    if (!info_line) {
        printf("No such file blueprint.txt\n");
        return 0;
    }

    new_map->lines = mx_atoi(get_word(info_line, 0));
    new_map->cols = mx_atoi(get_word(info_line, 1));
    new_map->block_w = mx_atoi(get_word(info_line, 2));
    new_map->block_h = mx_atoi(get_word(info_line, 3));
    new_map->texts_count = mx_atoi(get_word(info_line, 4));

    free(info_line);

    if (new_map->lines && new_map->cols && new_map->block_w &&
        new_map->block_h && new_map->texts_count) {
        return 1;
    }
    else {
        printf("Invalid info line data BLUEPRINT\n");
        return 0;
    }
}

// MAP_MAKE BLUEPRINT
static void scan_tiles(int file, t_map *new_map) {
    t_tile new_tile;
    char c;

    new_map->orks_left = 0;
    new_map->tokens_left = 0;
    for (int i = 0; i < new_map->lines; i++) {
        for (int j = 0; j < new_map->cols; j++) {
            read(file, &c, 1);

            new_tile.wall = false;
            new_tile.point = false;
            new_tile.power_up = false;
            new_tile.door = false;

            while (c != '#' && c != '*' && c != '^' && c != '@' &&
                   c != 'X' && c != 'Z' && c != ' ' && c != '.' &&
                   c != 'D')
                read(file, &c, 1);

            switch (c) {
                case 'D':
                    new_tile.door = true;
                    break;
                case 'Z':
                    new_map->ork[new_map->orks_left].x = j;
                    new_map->ork[new_map->orks_left].y = i;
                    new_map->orks_left++;
                    break;
                case '#':
                    new_tile.wall = true;
                    break;
                case '*':
                    new_map->tokens_left++;
                    new_tile.point = true;
                    break;
                case '^':
                    new_tile.power_up = true;
                    break;
                case '@':
                    new_map->entry_x = j;
                    new_map->entry_y = i;
                    break;
                case 'X':
                    new_map->exit_x = j;
                    new_map->exit_y = i;
                    break;
            }
            new_map->tiles[i][j] = new_tile;
        }
    }
}

static int init_tiles(t_map *new_map) {
    char *path = mx_strdup("resource/maps/level*/blueprint.txt");

    path[19] = new_map->level_num + '0';

    int file = open(path, O_RDONLY);

    free(path);

    if (file < 0) {
        printf("No such file blueprint.txt\n");
        return 0;
    }

    char c;

    while (read(file, &c, 1) && c != '\n') {
        c++;
    }

    new_map->tiles = malloc(new_map->lines * sizeof(t_tile *));
    for (int i = 0; i < new_map->lines; i++) {
        new_map->tiles[i] = malloc(new_map->cols * sizeof(t_tile));
    }

    scan_tiles(file, new_map);

    close(file);
    return 1;
}

// MAP_MAKE TEXTURES
static void path_set_filename(char *path, const char *filename) {
    while (*path != '*') {
        path++;
    }
    while (*filename) {
        *path = *filename;
        path++;
        filename++;
    }
    *path = '\0';
}

static int init_texts(t_map *new_map, SDL_Renderer *rend) {
    char *text_pathes[new_map->texts_count];
    const char *text_files_names[] = {"BACKGROUND.png",
                                      "WALL_BLOCK.png",
                                      "WALL_LINE.png",
                                      "WALL_T.png",
                                      "WALL_X.png",
                                      "WALL_CORNER.png",
                                      "WALL_END.png",
                                      "WALL_EMPY.png"};

    SDL_Surface *temp;
    new_map->texts =
        malloc(new_map->texts_count * sizeof(SDL_Texture *));

    for (int i = 0; i < new_map->texts_count; i++) {
        text_pathes[i] =
            mx_strdup("resource/maps/level*/******************");
        text_pathes[i][19] = new_map->level_num + '0';

        path_set_filename(text_pathes[i], text_files_names[i]);

        temp = IMG_Load(text_pathes[i]);
        if (!temp) {
            printf("No such img %s\n", text_pathes[i]);
            free(new_map->texts);
            new_map->texts = NULL;
            for (int i = 0; i < new_map->texts_count; i++) {
                free(text_pathes[i]);
            }
            return 0;
        }

        new_map->texts[i] = SDL_CreateTextureFromSurface(rend, temp);
        SDL_FreeSurface(temp);

        if (!new_map->texts[i]) {
            free(new_map->texts);
            printf("Create texture error %s\n", text_pathes[i]);
            new_map->texts = NULL;
            for (int i = 0; i < new_map->texts_count; i++) {
                free(text_pathes[i]);
            }
            return 0;
        }
    }
    return 1;
}

// MAP_MAKE REND_BLOCKS

static void back_block_make(t_map *new_map) {
    t_rend_blok *temp;

    temp = &(new_map->back);
    temp->text = new_map->texts[BACKGROUND];
    temp->angle = 0;
    temp->dest.w = new_map->cols * new_map->block_w;
    temp->dest.h = new_map->lines * new_map->block_h;

    temp->dest.x = new_map->offset_x;
    temp->dest.y = new_map->offset_y;
}

static void init_block(
    t_map *new_map, int i, int j, int e_text, double angle) {
    t_rend_blok *this_b = &(new_map->blocks[i][j]);

    this_b->is_configured = true;
    this_b->text = new_map->texts[e_text];
    this_b->dest.w = new_map->block_w;
    this_b->dest.h = new_map->block_h;
    this_b->dest.x = j * new_map->block_w + new_map->offset_x;
    this_b->dest.y = i * new_map->block_h + new_map->offset_y;

    this_b->angle = angle;
}

static void make_pedantic_wall(int i, int j, t_map *new_map) {
    t_tile **tiles = new_map->tiles;

    int maxi = new_map->lines - 1, maxj = new_map->cols - 1;

    bool up_wall, right_wall, left_wall, down_wall;

    up_wall = i == 0 ? true : tiles[i - 1][j].wall;
    right_wall = j == maxj ? true : tiles[i][j + 1].wall;
    left_wall = j == 0 ? true : tiles[i][j - 1].wall;
    down_wall = i == maxi ? true : tiles[i + 1][j].wall;

    bool corn_l_u, corn_l_d, corn_r_u, corn_r_d;

    corn_l_u = (i == 0 || j == 0) ? true : tiles[i - 1][j - 1].wall;
    corn_l_d = (i == maxi || j == 0) ? true : tiles[i + 1][j - 1].wall;
    corn_r_u = (i == 0 || j == maxj) ? true : tiles[i - 1][j + 1].wall;
    corn_r_d =
        (i == maxi || j == maxj) ? true : tiles[i + 1][j + 1].wall;

    int result = up_wall + right_wall + left_wall + down_wall;
    double angle = 0;
    int text = 1;

    switch (result) {
        case 0: {
            init_block(new_map, i, j, WALL_BLOCK, 0);
            break;
        }
        case 1: {  // WALL_END
            text = WALL_END;
            if (up_wall)
                angle = 180;
            else if (right_wall)
                angle = 270;
            else if (down_wall)
                angle = 0;
            else if (left_wall)
                angle = 90;
            break;
        }
        case 2: {  // WALL_LINE  // WALL_CORNER
            if ((up_wall && down_wall) || (left_wall && right_wall)) {
                text = WALL_LINE;
                if (up_wall)
                    angle = 0;
                else
                    angle = 90;
            }
            else {
                text = WALL_CORNER;
                if (!up_wall && !left_wall)
                    angle = 0;
                else if (!up_wall && !right_wall)
                    angle = 90;
                else if (!down_wall && !right_wall)
                    angle = 180;
                else if (!down_wall && !left_wall)
                    angle = 270;
            }
            break;
        }
        case 3: {  // WALL_T
            text = WALL_T;
            if (!up_wall) {
                if (corn_r_d && corn_l_d) {
                    text = WALL_LINE;
                    angle = 90;
                }
                else {
                    angle = 0;
                }
            }
            else if (!right_wall) {
                if (corn_l_u && corn_l_d) {
                    text = WALL_LINE;
                    angle = 0;
                }
                else {
                    angle = 90;
                }
            }
            else if (!down_wall) {
                if (corn_r_u && corn_l_u) {
                    text = WALL_LINE;
                    angle = 90;
                }
                else {
                    angle = 180;
                }
            }
            else if (!left_wall) {
                if (corn_r_d && corn_r_u) {
                    text = WALL_LINE;
                    angle = 0;
                }
                else {
                    angle = 270;
                }
            }
            break;
        }
        case 4: {  // WALL_X

            text = WALL_X;
            angle = 0;

            if (corn_l_u && corn_l_d && corn_r_u && corn_r_d) {
                text = WALL_EMPY;
                angle = 0;
            }
            else if (corn_l_u + corn_l_d + corn_r_u + corn_r_d == 3) {
                text = WALL_CORNER;
                if (!corn_l_u) {
                    angle = 180;
                }
                else if (!corn_l_d) {
                    angle = 90;
                }
                else if (!corn_r_u) {
                    angle = 270;
                }
                else if (!corn_r_d) {
                    angle = 0;
                }
            }
            else if (corn_l_u + corn_l_d + corn_r_u + corn_r_d == 2) {
                text = WALL_T;
                if (!corn_l_u && !corn_r_u) {
                    angle = 180;
                }
                else if (!corn_l_d && !corn_l_u) {
                    angle = 90;
                }
                else if (!corn_r_u && !corn_r_d) {
                    angle = 270;
                }
                else if (!corn_r_d && !corn_l_d) {
                    angle = 0;
                }
            }
            else {
                text = WALL_X;
                angle = 0;
            }

            break;
        }
    }

    init_block(new_map, i, j, text, angle);
}

static int init_rend_blocks(t_map *new_map, int win_w, int win_h) {
    new_map->offset_x = (win_w - new_map->cols * new_map->block_w) / 2;
    new_map->offset_y = (win_h - new_map->lines * new_map->block_h) / 2;

    back_block_make(new_map);

    new_map->blocks = malloc(new_map->lines * sizeof(t_rend_blok *));
    for (int i = 0; i < new_map->lines; i++) {
        new_map->blocks[i] =
            malloc(new_map->cols * sizeof(t_rend_blok));
    }

    for (int i = 0; i < new_map->lines; i++) {
        for (int j = 0; j < new_map->cols; j++) {
            if (new_map->texts_count <= 1) {
                new_map->blocks[i][j].is_configured = false;
                continue;
            }
            if (new_map->tiles[i][j].wall) {
                if (new_map->texts_count > 2) {
                    make_pedantic_wall(i, j, new_map);
                }
                else {
                    init_block(new_map, i, j, WALL_BLOCK, 0);
                }
            }
            else {
                new_map->blocks[i][j].is_configured = false;
            }
        }
    }
    return 1;
}

// MAP_RENDER
void map_render(t_map *map, SDL_Renderer *rend) {
    SDL_RenderCopy(rend, map->back.text, NULL, &(map->back.dest));

    for (int i = 0; i < map->lines; i++) {
        for (int j = 0; j < map->cols; j++) {
            if (map->blocks[i][j].is_configured) {
                SDL_RenderCopyEx(rend,
                                 map->blocks[i][j].text,
                                 NULL,
                                 &(map->blocks[i][j].dest),
                                 map->blocks[i][j].angle,
                                 NULL,
                                 SDL_FLIP_NONE);
            }
        }
    }
}

// MAP_FREE
void map_free(t_map *map) {
    if (map->tiles) {
        for (int i = 0; i < map->lines; i++) {
            if (map->tiles[i]) free(map->tiles[i]);
        }
        free(map->tiles);
        map->tiles = NULL;
    }

    if (map->texts) {
        for (int i = 0; i < map->texts_count; i++) {
            SDL_DestroyTexture(map->texts[i]);
        }
        free(map->texts);
        map->texts = NULL;
    }
    if (map->blocks) {
        for (int i = 0; i < map->lines; i++) {
            if (map->blocks[i]) free(map->blocks[i]);
        }
        free(map->blocks);
        map->blocks = NULL;
    }
}

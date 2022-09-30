#include "../inc/path_finder.h"
#include <time.h>

void renew_coords(t_orc * ork, int a_y, int a_x){
	ork->p_x = ork->x;
	ork->p_y = ork->y;
	ork->x += a_x;
	ork->y += a_y;	
}

bool is_back(t_orc * ork, int a_y, int a_x){
	if (a_y) return ork->y + a_y == ork->p_y;
	else if (a_x) return ork->x + a_x == ork->p_x;
	return -1;
}

int choose_random_cell(t_map * map, t_orc * ork){
	int r = rand() % 4;
	while (1){
		r = rand() % 4;
		if (r == 0 && !map->tiles[ork->y - 1][ork->x].wall) return 0;
		if (r == 1 && !map->tiles[ork->y + 1][ork->x].wall) return 1;	
		if (r == 2 && !map->tiles[ork->y][ork->x + 1].wall) return 2;
		if (r == 3 &&!map->tiles[ork->y][ork->x - 1].wall) return 3;
	}
	return -1;
}

void choose_dir(t_map * map, t_orc * ork){
	int b_y = ork->p_y - ork->y;
	int b_x = ork->p_x - ork->x;
	if (b_y != -1 && !map->tiles[ork->y - 1][ork->x].wall){
		renew_coords(ork, -1, 0);
	}
	else if (b_y != 1 && !map->tiles[ork->y + 1][ork->x].wall){
		renew_coords(ork, 1, 0);
	}
	else if (b_x != 1 && !map->tiles[ork->y][ork->x + 1].wall){
		renew_coords(ork, 0, 1);
	}
	else if (b_x != -1 && !map->tiles[ork->y][ork->x - 1].wall){
		renew_coords(ork, 0, -1);
	}
	else{
		renew_coords(ork, b_y, b_x);
	} 
}


int pathfinder(t_orc * ork, t_map * map, int s_y, int s_x){
    srand(time(0));
    int t_x;
    int t_y;
    if (ork->mode == 0){ //frightening or to prison
    	t_x = s_x;
    	t_y = s_y;
    }
    else if (ork->mode == 2){	
    	t_x = map->ork[ork->num].x;
    	t_y = map->ork[ork->num].y;
    }
    else{
    	t_x = rand() % (map->cols - s_x) + s_x;
    	t_y = rand() % (map->lines - s_y) + s_y; 
    }   
    int wave[map->lines + 2][map->cols + 2];
    for (int m = 0; m < map->lines + 2; m++)
        for (int n = 0; n < map->cols + 2; n++)
            wave[m][n] = -1;
    for (int m = 1; m < map->lines + 1; m++)
        for (int n = 1; n < map->cols + 1; n++)
            if (map->tiles[m - 1][n - 1].wall)
                wave[m][n] = -2;
    for (int m = 0; m < map->lines + 2; m++) {
        wave[m][0] = -2;
        wave[m][map->cols + 1] = -2;
    }
    for (int n = 0; n < map->cols + 2; n++) {
        wave[0][n] = -2;
        wave[map->lines + 1][n] = -2;
    }
    wave[t_y + 1][t_x + 1] = 0;
    int f = 0;
    int counter = 0;
    while(1) {
        f = 0;
        for (int m = 1; m < map->lines + 1; m++)
            for (int n = 1; n < map->cols + 1; n++)
                if (wave[m][n] == counter) {
                    if (wave[m - 1][n] == -1) {
                        wave[m - 1][n] = counter + 1;
                        f = 1;
                    }
                    if (wave[m + 1][n] == -1) {
                        wave[m + 1][n] = counter + 1;
                        f = 1;
                    }
                    if (wave[m][n - 1] == -1) {
                        wave[m][n - 1] = counter + 1;
                        f = 1;
                    }
                    if (wave[m][n + 1] == -1) {
                        wave[m][n + 1] = counter + 1;
                        f = 1;
                    }
                }
        if (f == 0)
            break;
	counter++;
    }
    int cur = wave[ork->y + 1][ork->x + 1];
    if (wave[ork->y + 1][ork->x] == cur - 1){
	    if (!map->tiles[ork->y][ork->x - 1].wall) return 3;	
    }
    else if (wave[ork->y + 1][ork->x + 2] == cur - 1){
	  if (!map->tiles[ork->y][ork->x + 1].wall) return 2;
    }
    else if (wave[ork->y][ork->x + 1] == cur - 1){
	 if (!map->tiles[ork->y - 1][ork->x].wall) return 0;
    }
    else if (wave[ork->y + 2][ork->x + 1] == cur - 1){
   	if (!map->tiles[ork->y + 1][ork->x].wall) return 1;
    }
    return choose_random_cell(map, ork);
}

int find_path(t_map * map, t_orc * ork, int s_y, int s_x, int dif){
		
	if (ork->mode == 2) return pathfinder(ork, map, s_y, s_x); 
	if (rand() % (dif + 1) == 0){
		return choose_random_cell(map, ork);
	}	
	return pathfinder(ork, map, s_y, s_x);
}

void move_ork(t_orc * ork1, SDL_Renderer *rend, t_map * map, int div)
{
	if (orc_render(ork1, rend)){
		if (div == 0){
			if (ork1->y - 1 != ork1->p_y) renew_coords(ork1, -1, 0);
			else choose_dir(map, ork1);
		}
		else if (div == 1){
			if (ork1->y + 1 != ork1->p_y) renew_coords(ork1, 1, 0);
		else choose_dir(map, ork1);
		}
		else if (div == 2){
			if (ork1->x + 1 != ork1->p_x) renew_coords(ork1, 0, 1);
			else choose_dir(map, ork1);
		}
		else if (div == 3){
			if (ork1->x - 1 != ork1->p_x) renew_coords(ork1, 0, -1);
		else choose_dir(map, ork1);
		}
	}
}

 

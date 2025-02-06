#include <curses.h>
#include <stdlib.h>
#include <time.h>

#include "../include/area.h"
#include "../include/snake.h"
#include "../include/food.h"

int check_food_collisions(struct food *f, struct snake *s)
{
	while(s) {
		if(s->cur_x == f->cur_x && s->cur_y == f->cur_y)
			return 1;
		s = s->next;
	}

	return 0;
}

void generate_piece_of_food(struct area *a, struct food **f, struct snake *s)
{
	int rx, ry, dx, dy;

	srand(time(NULL));
	dy = a->max_y - a->min_y - 1;
	dx = a->max_x - a->min_x - 1;
	rx = a->min_x + (int)((float)dx*rand()/(RAND_MAX + 1.0)) + 1;
	ry = a->min_y + (int)((float)dy*rand()/(RAND_MAX + 1.0)) + 1;

	if((*f) == NULL)
		(*f) = malloc(sizeof(struct food));
	
	(*f)->cur_x = rx;
	(*f)->cur_y = ry;
}

void delete_piece_of_food(struct food *f)
{
	free(f);
	f = NULL;
}

void show_food(struct food *f)
{
	move(f->cur_y, f->cur_x);
	addch('#');
	refresh();
}

void hide_food(struct food *f)
{
	move(f->cur_y, f->cur_x);
	addch(' ');
	refresh();
}

int check_food(struct food *f, int x, int y)
{
	if(f == NULL)
		return 0;
	return (f->cur_x == x) && (f->cur_y == y) ? 1 : 0;
}
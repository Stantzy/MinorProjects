#include <stdlib.h>
#include <curses.h>

#include "../include/area.h"
#include "../include/snake.h"

enum { head = '@', body = 'o' };

struct snake *create_snake(const struct area *a)
{
	struct snake *s, *tmp = NULL;
	int i;

	for(i = 0; i < start_size; i++) {
		if(tmp == NULL) {
			tmp = malloc(sizeof(struct snake));
			tmp->next = NULL;
			s = tmp;
		} else {
			tmp->next = malloc(sizeof(struct snake));
			tmp = tmp->next;
		}
		tmp->number = i;
		tmp->cur_x = a->min_x + (a->max_x - a->min_x) / 2 - i;
		tmp->cur_y = a->min_y + (a->max_y - a->min_y) / 2;
		tmp->dx = 1;
		tmp->dy = 0;
	}

	return s;
}

void delete_snake(struct snake *s)
{
	struct snake *tmp;
	tmp = s;
	while(tmp != NULL) {
		s = tmp->next;
		free(tmp);
		tmp = s;
	}
}

static void check_boundaries(int *coord, int min, int max)
{
	if(*coord < min + 1)
		*coord = max - 1;
	else
	if(*coord > max - 1)
		*coord = min + 1;
}

int check_snake_collisions(struct snake *s)
{
	struct snake *head;

	head = s;
	s = s->next;
	while(s) {
		if(head->cur_x == s->cur_x && head->cur_y == s->cur_y)
			return 1;
		s = s->next;
	}

	return 0;
}

static void delete_last_item(struct snake *s)
{
	struct snake *tmp;

	tmp = s;

	while(tmp != NULL) {
		if(tmp->next->next == NULL) {
			free(tmp->next);
			tmp->next = NULL;	
		}
		tmp = tmp->next;
	}
}

static void update_item_positions(struct snake *s)
{
	struct snake *tmp;
	int i;
	
	tmp = s;
	i = 0;
	while(tmp != NULL) {
		tmp->number = i;
		i++;
		tmp = tmp->next;
	}
}

static void update_snake(struct snake **s, int x, int y)
{
	struct snake *new_item;

	new_item = malloc(sizeof(struct snake));
	
	new_item->next = *s;
	new_item->dx = (*s)->dx;
	new_item->dy = (*s)->dy;
	new_item->cur_x = x;
	new_item->cur_y = y;

	delete_last_item(new_item);
	update_item_positions(new_item);

	*s = new_item;
}

void hide_snake(const struct snake *s)
{	
	while(s != NULL) {
		move(s->cur_y, s->cur_x);
		addch(' ');
		s = s->next;
	}
	refresh();
}

void show_snake(const struct snake *s)
{
	while(s != NULL) {
		move(s->cur_y, s->cur_x);
		if(s->number == 0)
			addch(head);
		else
			addch(body);
		s = s->next;
	}

	refresh();
}

void move_snake(struct snake **s, const struct area *a)
{
	struct snake *tmp;
	int y, x;
	
	tmp = *s;
	y = tmp->cur_y;
	x = tmp->cur_x;

	y += tmp->dy;
	x += tmp->dx;

	hide_snake(tmp);
	update_snake(&tmp, x, y);
	check_boundaries(&(tmp->cur_x), a->min_x, a->max_x);
	check_boundaries(&(tmp->cur_y), a->min_y, a->max_y);
	show_snake(tmp);

	*s = tmp;
}

void grow_snake(struct snake *s)
{
	struct snake *tmp, *new_item;
	
	new_item = malloc(sizeof(struct snake));

	tmp = s;
	while(tmp->next != NULL)
		tmp = tmp->next;
	tmp->next = new_item;
	new_item->dx = tmp->dx;
	new_item->dy = tmp->dy;
	new_item->cur_x = tmp->cur_x - tmp->dx;
	new_item->cur_y = tmp->cur_y - tmp->dy;
	update_item_positions(s);
}
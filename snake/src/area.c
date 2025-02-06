#include <curses.h>

#include "../include/area.h"

enum { vertical_wall = '|', horizontal_wall = '-' };

void show_area(struct area *a)
{
	int i, j;

	for(i = a->min_y; i <= a->max_y; i++) {
		for(j = a->min_x; j <= a->max_x; j++) {
			move(i, j);
			if(i == a->min_y || i == a->max_y)
				addch(horizontal_wall);
			else if(j == a->min_x || j == a->max_x)
				addch(vertical_wall);
		}
	}

	refresh();
}
#include <stdlib.h>
#include <curses.h>

#include "../include/area.h"
#include "../include/snake.h"
#include "../include/food.h"
#include "../include/game.h"

int main()
{
	struct area a;
	struct food *f = NULL;
	struct snake *s = NULL;
	int x, y, key;

	init_ncurses(&x, &y);
	a.min_x = x / 2 - area_width / 2 - 1;
	a.max_x = a.min_x + area_width;
	a.min_y = y / 2 - area_height / 2 - 1;
	a.max_y = a.min_y + area_height;

	s = create_snake(&a);

	show_area(&a);
	show_snake(s);
	generate_piece_of_food(&a, &f, s);
	move(a.max_y + 1, a.min_x);
	addstr("Press ESC to exit");

	while((key = getch()) != key_escape) {
		switch(key) {
			case(KEY_UP):
				set_direction(s, 0, -1);
				break;
			case(KEY_DOWN):
				set_direction(s, 0, 1);
				break;
			case(KEY_LEFT):
				set_direction(s, -1, 0);
				break;
			case(KEY_RIGHT):
				set_direction(s, 1, 0);
				break;
			case ERR:
				show_area(&a);
				score_counter(s, a.min_x, a.min_y - 2);
				show_food(f);
				move_snake(&s, &a);

				if(check_food(f, s->cur_x, s->cur_y)) {
					hide_food(f);
					grow_snake(s);
					generate_piece_of_food(&a, &f, s);
				}

				while(check_food_collisions(f, s))
					generate_piece_of_food(&a, &f, s);
				
				while(check_snake_collisions(s)) {
                    game_over(s, x, y);
					delete_snake(s);
					delete_piece_of_food(f);
				}
				break;
			default:
				break;
		}
	}
	
	delete_snake(s);
	delete_piece_of_food(f);
	endwin();
	return 0;
}
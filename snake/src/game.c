#include <time.h>
#include <curses.h>
#include <stdlib.h>

#include "../include/area.h"
#include "../include/snake.h"
#include "../include/game.h"

void init_ncurses(int *x, int *y)
{
	initscr();
	getmaxyx(stdscr, *y, *x);
	curs_set(0);
	cbreak();
	noecho();
	keypad(stdscr, 1);
	timeout(delay_duration);
	refresh();
}

void set_direction(struct snake *s, int dx, int dy)
{
	if(s->dx != -(dx))
		s->dx = dx;
	if(s->dy != -(dy))
		s->dy = dy;
}

void score_counter(struct snake *s, int x, int y)
{
	int score;

	while(s != NULL) {
		score = s->number;
		s = s->next;
	}

	move(y, x);
	addstr("Score: ");
	move(y, x + 7);
	printw("%d", score - start_size + 1);
	refresh();
}

void game_over(struct snake *s, int x, int y)
{
	int key;
	enum { horizontal_shift = 10 };

	clear();

	move(y / 2 - 3, x / 2 - horizontal_shift);
	addstr("Game Over");

	score_counter(s, x / 2 - horizontal_shift, y / 2 - 2);
	
	move(y / 2 - 1, x / 2 - horizontal_shift);
	addstr("Press ESC to exit");
	
	refresh();
	while((key = getch()) != key_escape) {}
	endwin();
	
	exit(1);
}
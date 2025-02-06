#ifndef GAME_H_SENTRY
#define GAME_H_SENTRY

enum { delay_duration = 100 };
enum { key_escape = 27 };

void init_ncurses(int *x, int *y);
void set_direction(struct snake *s, int dx, int dy);
void score_counter(struct snake *s, int x, int y);
void game_over(struct snake *s, int x, int y);

#endif
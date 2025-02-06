#ifndef SNAKE_H_SENTRY
#define SNAKE_H_SENTRY

enum { start_size = 5 };

struct snake {
	int number;
	int cur_x, cur_y, dx, dy;
	struct snake *next;
};

struct snake *create_snake(const struct area *a);
void delete_snake(struct snake *s);
int check_snake_collisions(struct snake *s);
void hide_snake(const struct snake *s);
void show_snake(const struct snake *s);
void move_snake(struct snake **s, const struct area *a);
void grow_snake(struct snake *s);

#endif
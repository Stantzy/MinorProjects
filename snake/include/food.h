#ifndef FOOD_H_SENTRY
#define FOOD_H_SENTRY

struct food {
	int cur_x, cur_y;
};

int check_food_collisions(struct food *f, struct snake *s);
void generate_piece_of_food(struct area *a, struct food **f, struct snake *s);
void delete_piece_of_food(struct food *f);
void show_food(struct food *f);
void hide_food(struct food *f);
int check_food(struct food *f, int x, int y);

#endif
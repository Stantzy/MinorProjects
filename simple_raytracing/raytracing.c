#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef DISPLAY_WIDTH
#define DISPLAY_WIDTH 640
#endif

#ifndef DISPLAY_HEIGHT
#define DISPLAY_HEIGHT 480
#endif

#ifndef RAYS_NUMBER
#define RAYS_NUMBER 100
#endif

#ifndef RAY_THICKNESS
#define RAY_THICKNESS 1
#endif

#ifndef D_X
#define D_X 0
#endif

#ifndef D_Y
#define D_Y 1
#endif

#define CIRCLES_COUNT 3
#define SDL_DELAY 10
#define COLOR_WHITE 0xffffffff
#define COLOR_GRAY 0xb7b7b7b7
#define COLOR_BLACK 0x00000000
#define COLOR_RAY 0xffe85f

struct circle {
	double x;
	double y;
	double r;
};

struct ray {
	double x, y;
	double angle;
};

void exit_with_error()
{
	fprintf(stderr, "Error: %s\n", SDL_GetError());
	exit(1);
}

void draw_circle
(SDL_Surface *surface, struct circle c, Uint32 color)
{
	double x, y;
	double distance_squared;
	double radius_squared = pow(c.r, 2);

	for(x = c.x - c.r; x <= c.x + c.r; x++) {
		for(y = c.y - c.r; y <= c.y + c.r; y++) {
			distance_squared = pow(x - c.x, 2) + pow (y - c.y, 2);
			if(distance_squared < radius_squared) {
				SDL_Rect pixel = (SDL_Rect) { x, y, 1, 1 };
				SDL_FillRect(surface, &pixel, color);
			}
		}
	}
}

void handle_event(int *simulation_running, struct circle *c)
{
	SDL_Event event;

	while(SDL_PollEvent(&event)) {
		switch(event.type) {
		case SDL_QUIT:
			*simulation_running = 0;
			break;
		case SDL_MOUSEMOTION:
			if(event.motion.state == SDL_PRESSED) {
				c->x = event.motion.x;
				c->y = event.motion.y;
			}
			break;
		default:
			break;
		}	
	}
}

void generate_rays(struct circle c, struct ray rays[RAYS_NUMBER])
{
	double angle;

	for(int i = 0; i < RAYS_NUMBER; i++) {
		angle = ((double)i / RAYS_NUMBER) * 2 * M_PI;
		rays[i].x = c.x;
		rays[i].y = c.y;
		rays[i].angle = angle;
	}
}

int check_collisions
(double x_cur, double y_cur, struct circle c[], int size)
{
	for(int i = 0; i < size; i++) {
		double distance_squared =
			pow(c[i].x - x_cur, 2) + pow (c[i].y - y_cur, 2);
		double radius_squared = pow(c[i].r, 2);
		if(distance_squared < radius_squared)
			return 1;
	}

	return 0;
}

void draw_rays
(SDL_Surface *s, struct ray rays[], struct circle c[], int size, Uint32 color)
{

	SDL_Rect pixel = (SDL_Rect) { 0, 0, RAY_THICKNESS, RAY_THICKNESS };

	for(int i = 0; i < RAYS_NUMBER; i++) {
		int end_of_screen = 0;
		int hit = 0;
		double step = 1;
		double x_draw = rays[i].x;
		double y_draw = rays[i].y;

		while(!end_of_screen && !hit) {
			x_draw += step * cos(rays[i].angle);
			y_draw += step * sin(rays[i].angle);
		
			hit = check_collisions(x_draw, y_draw, c, size);
			if(x_draw <= 0 || x_draw >= DISPLAY_WIDTH)
				end_of_screen = 1;
			if(y_draw <= 0 || y_draw >= DISPLAY_HEIGHT)
				end_of_screen = 1;
	
			pixel.x = x_draw;
			pixel.y = y_draw;

			SDL_FillRect(s, &pixel, color);
		}
	}
}

void move_circle(SDL_Surface *s, struct circle *c, int *d_y, int *d_x)
{
	if(c->y + c->r >= s->h)
		*d_y = -D_Y;
	else if(c->y - c->r <= 0)
		*d_y = D_Y;

	if(c->x + c->r >= s->w)
		*d_x = -D_X;
	else if(c->x - c->r <= 0)
		*d_x = D_X;

	c->y += *d_y;
	c->x += *d_x;
}

int main()
{
	int ok;
	SDL_Window *window;
	SDL_Surface *surface;
	SDL_Rect erase_rect = { 0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT };
	struct circle cir = { 150, 300, 25 };
	struct circle shadow_circles[CIRCLES_COUNT];
	struct circle static_circle_0 = { 300, 150, 50 };
	struct circle static_circle_1 = { 100, 200, 25 };
	struct circle static_circle_2 = { 500, 300, 75 };
	struct ray rays[RAYS_NUMBER];
	int d_y = D_Y;
	int d_x = D_X;

	shadow_circles[0] = static_circle_0;
	shadow_circles[1] = static_circle_1;
	shadow_circles[2] = static_circle_2;

	if(SDL_Init(SDL_INIT_VIDEO) != 0) {
		fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
		exit(1);
	}

	/* Clean up on exit */
	ok = atexit(SDL_Quit);
	if(ok != 0) {
		fprintf(stderr, "Cannot set exit function\n");
		SDL_Quit();
		exit(1);
	}

	window = SDL_CreateWindow(
			"Graphics",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			DISPLAY_WIDTH,
			DISPLAY_HEIGHT,
			SDL_WINDOW_SHOWN
		);
	if(window == NULL)
		exit_with_error();

	surface = SDL_GetWindowSurface(window);
	if(surface == NULL)
		exit_with_error();


	int simulation_running = 1;
	while(simulation_running) {
		handle_event(&simulation_running, &cir);
		SDL_FillRect(surface, &erase_rect, COLOR_BLACK);
		generate_rays(cir, rays);
		draw_rays(
			surface,
			rays,
			shadow_circles,
			CIRCLES_COUNT,
			COLOR_RAY
		);
		draw_circle(surface, cir, COLOR_WHITE);
		for(int i = 0; i < CIRCLES_COUNT; i++)
			draw_circle(surface, shadow_circles[i], COLOR_WHITE);
		move_circle(surface, &shadow_circles[0], &d_y, &d_x);
		SDL_UpdateWindowSurface(window);
		SDL_Delay(SDL_DELAY);
	}

	SDL_DestroyWindow(window);

	return 0;
}

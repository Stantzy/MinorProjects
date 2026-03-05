#include <stdio.h>
#include <math.h>
#include <time.h>
#include "raylib.h"

#define WIDTH 600
#define HEIGHT 600

#ifndef CLOCK_CONSTANTS
#define CLOCK_CONSTANTS

#define FULL_CIRCLE_DEGREES 360.0f
#define HOURS_PER_CIRCLE 12
#define MINUTES_PER_CIRCLE 60
#define SECONDS_PER_CIRCLE 60

#define CLOCK_FACE_RADIUS_RATIO 0.4f
#define CLOCK_ANGLE_CORRECTION -90.0f

#define MARKER_START_RATIO 0.9f
#define MARKER_END_RATIO 0.98f
#define MARKER_WIDTH 2

#define HOUR_HAND_LENGTH_RATIO 0.5
#define MINUTE_HAND_LENGTH_RATIO 0.7
#define SECOND_HAND_LENGTH_RATIO 0.85

#define HOUR_HAND_WIDTH 5
#define MINUTE_HAND_WIDTH 3
#define SECOND_HAND_WIDTH 1

#endif

struct circle {
	float radius;
	int x_pos;
	int y_pos;
};

struct clock {
	struct circle clockface;
	int hour_hand_length;
	int minute_hand_length;
	int second_hand_length;
};

void init_circle(struct circle *c)
{
	c->radius = HEIGHT * CLOCK_FACE_RADIUS_RATIO;
	c->x_pos = WIDTH / 2;
	c->y_pos = HEIGHT / 2;
}

void init_clock(struct clock *clk)
{
	float radius;

	init_circle(&clk->clockface);
	radius = clk->clockface.radius;
	clk->hour_hand_length = radius * HOUR_HAND_LENGTH_RATIO;
	clk->minute_hand_length = radius * MINUTE_HAND_LENGTH_RATIO;
	clk->second_hand_length = radius * SECOND_HAND_LENGTH_RATIO;
}

void draw_minute_markers(struct clock clk)
{
	struct circle cf = clk.clockface;
	float alpha_deg = 0;

	for(int i = 0; i < MINUTES_PER_CIRCLE; i++) {
		float alpha_rad = alpha_deg * DEG2RAD;

		float x_inner = cf.radius * MARKER_START_RATIO * sinf(alpha_rad) + cf.x_pos;
		float y_inner = cf.radius * MARKER_START_RATIO * cosf(alpha_rad) + cf.y_pos;
		Vector2 inner = { x_inner, y_inner };

		float x_outer = cf.radius * MARKER_END_RATIO * sinf(alpha_rad) + cf.x_pos;
		float y_outer = cf.radius * MARKER_END_RATIO * cosf(alpha_rad) + cf.y_pos;
		Vector2 outer = { x_outer, y_outer };

		alpha_deg += FULL_CIRCLE_DEGREES / MINUTES_PER_CIRCLE;

		DrawLineEx(inner, outer, MARKER_WIDTH, GRAY);
	}
}

void draw_hour_hand(struct clock clk, struct tm *t)
{
	struct circle cf = clk.clockface;

	float degrees_per_hour = FULL_CIRCLE_DEGREES / HOURS_PER_CIRCLE;
	float minute_alpha_progress = (t->tm_min) / (float) MINUTES_PER_CIRCLE;
	float minute_offset = minute_alpha_progress * degrees_per_hour;
	float alpha_deg =
		((float) (t->tm_hour % HOURS_PER_CIRCLE)) * degrees_per_hour
		+ minute_offset;
	float alpha_rad = (alpha_deg + CLOCK_ANGLE_CORRECTION) * DEG2RAD;

	Vector2 center = { cf.x_pos, cf.y_pos };

	float x_outer = cf.x_pos + clk.hour_hand_length * cosf(alpha_rad);
	float y_outer = cf.y_pos + clk.hour_hand_length * sinf(alpha_rad);
	Vector2 outer = { x_outer, y_outer };

	DrawLineEx(center, outer, HOUR_HAND_WIDTH, RED);
}

void draw_minute_hand(struct clock clk, struct tm *t)
{
	struct circle cf = clk.clockface;

	float degrees_per_minute = FULL_CIRCLE_DEGREES / MINUTES_PER_CIRCLE;
	float second_alpha_progress = (t->tm_sec) / (float) SECONDS_PER_CIRCLE;
	float second_offset = second_alpha_progress * degrees_per_minute;
	float alpha_deg =
		((float) (t->tm_min % MINUTES_PER_CIRCLE)) * degrees_per_minute
		+ second_offset;
	float alpha_rad = (alpha_deg + CLOCK_ANGLE_CORRECTION) * DEG2RAD;

	Vector2 center = { cf.x_pos, cf.y_pos };

	float x_outer = cf.x_pos + clk.minute_hand_length * cosf(alpha_rad);
	float y_outer = cf.y_pos + clk.minute_hand_length * sinf(alpha_rad);
	Vector2 outer = { x_outer, y_outer };

	DrawLineEx(center, outer, MINUTE_HAND_WIDTH, BLACK);
}

void draw_second_hand(struct clock clk, struct tm *t)
{
	struct circle cf = clk.clockface;

	float degrees_per_second = FULL_CIRCLE_DEGREES / SECONDS_PER_CIRCLE;
	float alpha_deg =
		((float) (t->tm_sec % SECONDS_PER_CIRCLE)) * degrees_per_second;
	float alpha_rad = (alpha_deg + CLOCK_ANGLE_CORRECTION) * DEG2RAD;

	Vector2 center = { cf.x_pos, cf.y_pos };

	float x_outer = cf.x_pos + clk.second_hand_length * cosf(alpha_rad);
	float y_outer = cf.y_pos + clk.second_hand_length * sinf(alpha_rad);
	Vector2 outer = { x_outer, y_outer };

	DrawLineEx(center, outer, SECOND_HAND_WIDTH, BLACK);
}

void clock_run(struct clock *clk)
{
	struct circle cf = clk->clockface;
	time_t now;
	struct tm *t;

	InitWindow(WIDTH, HEIGHT, "Simple Analog Clock");

	while (!WindowShouldClose()) {
		now = time(NULL);
		t = localtime(&now);

		BeginDrawing();

		ClearBackground(BLACK);
		DrawCircle(cf.x_pos, cf.y_pos, cf.radius, RAYWHITE);
	
		draw_minute_markers(*clk);
		draw_hour_hand(*clk, t);
		draw_minute_hand(*clk, t);
		draw_second_hand(*clk, t);

		EndDrawing();
	}

	CloseWindow();
}

int main(int argc, char **argv)
{
	struct clock clk;

	init_clock(&clk);
	clock_run(&clk);

	return 0;
}


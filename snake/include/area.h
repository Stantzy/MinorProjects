#ifndef AREA_H_SENTRY
#define AREA_H_SENTRY

enum { area_width = 50 , area_height = 25 };

struct area {
	int min_x, max_x, min_y, max_y;
};

void show_area(struct area *a);

#endif
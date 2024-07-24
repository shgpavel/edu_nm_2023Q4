#ifndef DRAW_H
#define DRAW_H

#include "../types/pair.h"
#include "../types/vector.h"

void add_func(vector *);
void add_point(pair *);
void add_spline_func(vector *, vector *);
void str_func(char *);
void plot(size_t);
void clear_plot(void);

#endif

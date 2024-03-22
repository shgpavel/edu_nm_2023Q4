#ifndef DRAW_H
#define DRAW_H

FILE* draw_init(vector *);
void draw_another(vector *target, FILE *stream);
void draw_last(vector *target, FILE *stream);
void draw_func(char *p, FILE *stream);

#endif

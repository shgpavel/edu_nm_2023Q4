#include <stdio.h>
#include <string.h>
#include <jemalloc/jemalloc.h>

#include "../common.h"
#include "../types/vector.h"

#define str_limit 20000

char *go_str(vector *target) {
  char *equation = malloc(str_limit);
  char *ptr = equation;

  for (size_t i = 0; i < target->size; ++i) {
    sprintf(ptr, "%lg*x**%zu", 
                    unwrap_double(vector_get(target, i)), i);
    ptr += strlen(ptr);
    if ((i < target->size - 1) &&
        unwrap_double(vector_get(target, i + 1)) > 0) {
      sprintf(ptr, "%s", "+");
      ptr += strlen(ptr);
    }
  }
  return equation; 
}

FILE* draw_init(vector *target) {
  char *p = go_str(target);
 
  FILE *gnuplotPipe = popen("gnuplot -persistent", "w");
  if (!gnuplotPipe) {
    fprintf(stderr, "Error: Could not draw a GNU Plot graph\n");
    exit(1);
  }

  fprintf(gnuplotPipe, "set xlabel 'x'\n");
  fprintf(gnuplotPipe, "set ylabel 'y'\n");
  fprintf(gnuplotPipe, "set grid\n");
  fprintf(gnuplotPipe, "set term x11 title 'Graph'\n");

  fprintf(gnuplotPipe, "plot %s", p);
  free(p);
  return gnuplotPipe;
}

void draw_func(char *p, FILE *stream) {
  fprintf(stream, ", %s", p);
}

void draw_another(vector *target, FILE *stream) {
  char *p = go_str(target);
  fprintf(stream, ", %s", p);
  free(p);
}

void draw_last(vector *target, FILE *stream) {
  char *p = go_str(target);
  fprintf(stream, ", %s\n", p);
  fflush(stream);
  free(p);
  pclose(stream);
}

void draw_close(FILE *stream) {
  fprintf(stream, "\n");
  fflush(stream);
  pclose(stream);
}

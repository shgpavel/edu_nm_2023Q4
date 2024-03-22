#include <stdio.h>
#include <string.h>

#include "../common.h"
#include "../types/vector.h"

#define str_limit 39000

int draw(vector *target, int num) {
  static char *helv[str_limit] = malloc(50 * str_limit);
  static int j = 1;

  char equation[str_limit];
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
  ++j;


  if (window == 0) {
    FILE *gnuplotPipe = popen("gnuplot -persistent", "w");
    if (!gnuplotPipe) {
      fprintf(stderr, "Error: Could not draw a GNU Plot graph\n");
      return -1;
    }

    fprintf(gnuplotPipe, "set xlabel 'x'\n");
    fprintf(gnuplotPipe, "set ylabel 'y'\n");
    fprintf(gnuplotPipe, "set grid\n");
    fprintf(gnuplotPipe, "set term x11 title 'Graph'\n");
  }

  fprintf(gnuplotPipe, "plot %s with lines smooth csplines\n", equation);
  fflush(gnuplotPipe);
  
  if (window == -1) {
    pclose(gnuplotPipe);
  }

  return 0;
}

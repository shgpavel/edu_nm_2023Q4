#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../common.h"
#include "../types/vector.h"

#define str_limit 20000

int draw(vector *target) {
  static int is_window = 0;
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
  printf("%s\n", equation);

  FILE *gnuplotPipe = popen("gnuplot -persistent", "w");
  if (!gnuplotPipe) {
    fprintf(stderr, "Error: Could not draw a GNU Plot graph\n");
    return -1;
  }
  fprintf(gnuplotPipe, "set term x11 title 'Graph'\n");
  fprintf(gnuplotPipe, "plot %s with lines smooth csplines\n", equation);
  fflush(gnuplotPipe);

  return 0;
}

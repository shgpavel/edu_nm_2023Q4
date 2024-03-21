#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "types/vector.h"
#include "types/matrix.h"
#include "methods/polynoms.h"
#include "methods/lup.h"
#include "draw/draw.h"

int main(void) {

  size_t s_1, s_2;
  vector p_1, p_2;

  printf("poly degs?\n");
  scanf("%zu", &s_1);
  scanf("%zu", &s_2);

  vector_init(&p_1, s_1 + 1, sizeof(double));
  vector_init(&p_2, s_2 + 1, sizeof(double));

  printf("first poly?\n");
  for (size_t i = 0; i <= s_1; ++i) {
    double tmp;
    scanf("%lf", &tmp);
    vector_push(&p_1, &tmp);
  }

  printf("sec poly?\n");
  for (size_t i = 0; i <= s_2; ++i) {
    double tmp;
    scanf("%lf", &tmp);
    vector_push(&p_2, &tmp);
  }

  vector result = poly_mult(&p_1, &p_2);
  vector_print(&result);

  draw(&result);

  vector_free(&p_1);
  vector_free(&p_2);
  vector_free(&result);

  return 0;
}

#include <stdio.h>

#include "../types/vector.h"
#include "../draw/draw.h"

int main() {
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

  FILE *f = draw_init(&result);

  vector_free(&p_1);
  vector_free(&p_2);
  vector_free(&result);

  vector pol_1, pol_2;

  printf("poly degs?\n");
  scanf("%zu", &s_1);
  scanf("%zu", &s_2);

  vector_init(&pol_1, s_1 + 1, sizeof(double));
  vector_init(&pol_2, s_2 + 1, sizeof(double));

  printf("first poly?\n");
  for (size_t i = 0; i <= s_1; ++i) {
    double tmp;
    scanf("%lf", &tmp);
    vector_push(&pol_1, &tmp);
  }

  printf("sec poly?\n");
  for (size_t i = 0; i <= s_2; ++i) {
    double tmp;
    scanf("%lf", &tmp);
    vector_push(&pol_2, &tmp);
  }

  vector result_ = poly_mult(&pol_1, &pol_2);

  draw_another(&result_, f);

  char *func = "tan(pi/2 - x) - x";
  draw_func(func, f);
  draw_last(&result_, f);

  vector_free(&pol_1);
  vector_free(&pol_2);
  vector_free(&result_);
  return 0;
}

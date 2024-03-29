#include <stdio.h>

#include "types/vector.h"
#include "types/matrix.h"
#include "types/pair.h"
#include "methods/polynoms.h"
#include "methods/lup.h"
#include "draw/draw.h"

#include "common.h"

int main(void) {
  
  /*
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
  */

  size_t s;
  scanf("%zu", &s);
  
  vector points;
  vector_init(&points, s, sizeof(pair));

  for (size_t i = 0; i < s; ++i) {
    pair tmp;
    scanf("%lf", &tmp.a);
    vector_push(&points, &tmp);
  }

  for (size_t i = 0; i < s; ++i) {
    double tmp;
    scanf("%lf", &tmp);
    pair tmp_;
    tmp_.b = tmp;
    tmp_.a = unwrap_pair(vector_get(&points, i)).a;
    vector_change(&points, i, &tmp_);
  }
  
  //vector_print_pairs(&points);
  
  vector res = lagrange_poly(&points);
  vector_print(&res);

  vector_free(&res);
  vector_free(&points);

  return 0;
}

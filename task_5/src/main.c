#include <stdio.h>

#include "types/vector.h"
#include "types/matrix.h"
#include "types/pair.h"
#include "methods/polynoms.h"
#include "methods/lup.h"
#include "draw/draw.h"

#include "common.h"

int main(void) {
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
  
  vector res = lagrange_poly(&points);
  vector_print(&res);

  vector_free(&points);
  
  FILE *f = draw_init(&res);
  char *func = "tan(pi/2 - x) - x";
  draw_func(func, f);
  draw_close(f);


  vector_free(&res);

  res = lagrange_poly(&points);
  vector_print(&res);
  vector_free(&res);
  return 0;
}

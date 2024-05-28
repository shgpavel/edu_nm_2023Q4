#include <stdio.h>

#include <jemalloc/jemalloc.h>

#include "types/vector.h"
#include "types/pair.h"
#include "methods/lagrange.h"
#include "methods/newton.h"
#include "methods/linear_spline.h"
#include "methods/quad_spline.h"
#include "draw/draw.h"
#include "funcs/funcs.h"


void test() {
  size_t sz;
  scanf("%zu", &sz);
  vector points;
  vector_init(&points, sz, sizeof(pair));

  for (size_t i = 0; i < sz; ++i) {
    pair pushed;
    scanf("%lf", &pushed.a);
    vector_push(&points, &pushed);
  }

  for (size_t i = 0; i < sz; ++i) {
    scanf("%lf", &(pair_get(&points, i)).b);
  }
  vector_print_pairs(&points);

  vector *res = newton_poly(&points);
  vector_print(res);
  vector_free(res);
  free(res);
}

void test_spline() {
  size_t sz;
  scanf("%zu", &sz);
  vector points;
  vector_init(&points, sz, sizeof(pair));

  for (size_t i = 0; i < sz; ++i) {
    pair pushed;
    scanf("%lf", &pushed.a);
    vector_push(&points, &pushed);
  }

  for (size_t i = 0; i < sz; ++i) {
    scanf("%lf", &(pair_get(&points, i)).b);
  }
  vector_print_pairs(&points);

  vector *res = quad_spline(&points);
  add_spline_func(res, &points);
  
  for (size_t i = 0; i < res->size; ++i) {
    vector_print(vector_get(res, i));
    vector_free(vector_get(res, i));
    free(vector_get(res, i));
  }

  plot(1);
  clear_plot();
  
  vector_free(res);
  free(res);
}

int main(void) {

  test_spline();
  /*  
  pair segm = {1.0, 2.542};

  str_func("x = 1");
  str_func("x = 2.542");
  
  // evenly distributed nodes  
  for (size_t s = 14; s <= 17; ++s) {
    vector points;
    vector_init(&points, s, sizeof(pair));
    for (size_t i = 0; i < s; ++i) {
      double point = segm.a 
        + (double)i * ((segm.b - segm.a) / (double)(s - 1));
      pair point_p = {point, func(point)};
      vector_push(&points, (void *)&point_p);
    }

    vector *res = lagrange_poly(&points);
    vector_free(&points);
    
    add_func(res);
    vector_free(res);
  }
  */  

  /* optimal nodes */
  /*
  for (size_t s = 20; s <= 40; ++s) {
    vector points;
    vector_init(&points, s, sizeof(pair));
    for (size_t i = 0; i < s; ++i) {
      double point = opt_point(segm, i, s);
      pair point_p = {point, func(point)};
      vector_push(&points, (void *)&point_p);
    }
    vector *res = lagrange_poly(&points);
    vector_free(&points);

    vector_print(res);
    
    add_func(res);
    vector_free(res);
    free(res);
  }
  */
  //plot();
  //clear_plot();
  return 0;
}

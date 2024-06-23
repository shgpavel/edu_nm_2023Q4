#include <stdio.h>
#include <math.h>
#include <time.h>

#include <unistd.h>
#include <jemalloc/jemalloc.h>

#include "funcs/func.h"
#include "types/vector.h"
#include "types/matrix.h"
#include "types/pair.h"
#include "methods/polynoms.h"
#include "methods/orth_poly.h"
#include "methods/normal_poly.h"
#include "draw/draw.h"
#include "common.h"

#define CONTROL_POINTS 100
#define M_POINTS 100
#define at_one_point 3


/*
  This func needs an obvious optimization but
  I already made such crap arch so it will continue
  to stay just as shame board
*/
vector* entry(size_t n, size_t m, size_t flag) {  
  matrix E;
  vector f;
  matrix_init(&E, m * at_one_point, n + 1, sizeof(double));
  vector_init(&f, m * at_one_point, sizeof(double));

  vector *res = NULL;
  static size_t flag_orth = 0;
  static vector *orth;

  
  double add = (BOUND_B - BOUND_A) / ((double) m);
  double x = -1;

  size_t point_flag = 1;
  for (size_t i = 0; i < m * at_one_point; ++i) {
    double val;
    switch (point_flag) {
      case 1:
        val = func(x) - 3 * addition;
        break;
      case 2:
        val = func(x) + 6 * addition;
        break;
      case 3:
        val = func(x) - addition;
        break;
    }
    vector_push(&f, (void  *)&val);
    
    for (size_t j = 0; j < E.cols; ++j) {
      double xinj = pow(x, ((double)j));
      matrix_push(&E, (void *)&xinj);
    }
    point_flag = (point_flag % 3) + 1;
    if (point_flag == 1) x += add;
  }
    
  if (flag == 0) { 

    res = normal_equations(&E, &f);

  } else if (flag == 1) {
    if (flag_orth == 0) {
      orth = orth_init(n, &E);
      flag_orth = 1;
    }
    
    res = orth_step(n, &E, &f, orth);

  } else if (flag == 3) {
    matrix_free(&E);
    vector_free(&f);
    return orth;
  } else if (flag == 4) {
    for (size_t i = 0; i < E.rows; ++i) {
      pair point = {matrix_val(&E, i, 1), vector_val(&f, i)};
      add_point(&point);
    }    
  }

  matrix_free(&E);
  vector_free(&f);
  return res;
}

int main(void) {
  
  FILE *csv = fopen("out.csv", "w");
  if (!csv) return -1;
  fprintf(csv, "n, sigma_ne, sigma_ore\n");

  pair min_a = {1.0, 0.0}, min_b = {1.0, 0.0};
  for (size_t n = 2; n < 100; ++n) {
    vector *a = entry(n - 1, M_POINTS, 0);
    vector *b = entry(n, M_POINTS, 1);
    
    double add = (BOUND_B - BOUND_A) / CONTROL_POINTS;
    double x = -1;

    double sum_a = 0.0, sum_b = 0.0;
    for (size_t i = 0; i < CONTROL_POINTS; ++i, x += add) {
      sum_a += pow((poly_val(a, x) - func(x)), 2);
      sum_b += pow((poly_val(b, x) - func(x)), 2);
    }

    min_a = sum_a < min_a.a ? (pair){sum_a, n - 1} : min_a;
    min_b = sum_b < min_b.a ? (pair){sum_b, n} : min_b;
    
    fprintf(csv, "%zu, %lg, %lg\n", n - 1, sum_a, sum_b);
    
    vector_free(a);
    vector_free(b);
    free(a);
    free(b);
  }
  fclose(csv);
  
  vector *orth = entry(0, 0, 3);

  for (size_t i = orth->size - 1; i > (size_t)min_b.b; --i) {
    vector_free(vector_get(orth, i));
    free(vector_get(orth, i));
    orth->size--;
  }
  
  printf("\n%zu %zu\n", (size_t)min_a.b, (size_t)min_b.b);
  vector *best_ne = entry((size_t)min_a.b, M_POINTS, 0);
  vector *best_ore = entry((size_t)min_b.b, M_POINTS, 1);

  add_func(best_ne);
  add_func(best_ore);

  entry(1, M_POINTS, 4);

  plot();
  sleep(1);
  clear_plot();
  
  vector_free(best_ne);
  vector_free(best_ore);
  free(best_ne);
  free(best_ore);

  for (size_t i = orth->size - 1; i > 5; --i) {
    vector_free(vector_get(orth, i));
    free(vector_get(orth, i));
    orth->size--;
  }

  size_t orth_size = orth->size;
  for (size_t i = 5; i >= 1; --i, orth->size--) {
    vector *norm_i = entry(i, M_POINTS, 0);
    vector *orth_i = entry(i, M_POINTS, 1);

    add_func(norm_i);
    add_func(orth_i);
    
    vector_free(norm_i);
    vector_free(orth_i);
    free(norm_i);
    free(orth_i);
  }
  orth->size = orth_size;

  entry(1, M_POINTS, 4);
  plot();
  clear_plot();
  
  for (int i = (int)orth->size - 1; i >= 0; --i) {
    vector_free(vector_get(orth, i));
    free(vector_get(orth, i));
    if (orth->size != 0) orth->size--;
  }
  
  vector_free(orth);
  free(orth);
  
  return 0;
}

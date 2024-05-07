#include <stdio.h>

#include <unistd.h>
#include <jemalloc/jemalloc.h>

#include "funcs/func.h"
#include "types/vector.h"
#include "types/matrix.h"
#include "types/pair.h"
#include "methods/gauss.h"
#include "methods/polynoms.h"
#include "draw/draw.h"
#include "common.h"


vector* normal_equations(matrix *, vector *);
vector* orth_init(size_t, matrix *);
vector* orth_step(size_t, matrix *, vector *, vector *);

#define CONTROL_POINTS 100
#define M_POINTS 100

vector* entry(size_t n, size_t m, size_t flag) {
  matrix E;
  vector f;
  matrix_init(&E, m, n + 1, sizeof(double));
  vector_init(&f, m, sizeof(double));
  
  double add = (BOUND_B - BOUND_A) / ((double) m);
  double x = -1;

  for (size_t i = 0; i < m; ++i, x += add) {
    double tmp = func(x) - 3 * addition;
    vector_push(&f, (void *)&tmp);
  
    for (size_t j = 0; j < E.cols; ++j) {
      double tmp = pow(x, ((double)j));
      matrix_push(&E, (void *)&tmp);
    }
  }

  vector *res = NULL;
  static size_t flag_orth = 0;
  static vector *orth;

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
  }
  
  matrix_free(&E);
  vector_free(&f);
  return res;
}

vector* normal_equations(matrix *E, vector *f) {
  matrix *E_tr = matrix_transpose(E);
  
  matrix *left = matrix_on_matrix(E_tr, E);
  vector *right = matrix_on_vector(E_tr, f);

  matrix_free(E_tr);
  free(E_tr);

  vector *res = gauss(left, right);

  vector_free(right);
  matrix_free(left);
  free(left);
  free(right);

  return res;
}

vector* orth_init(size_t n, matrix *E) {
  vector *orth_poly = (vector *)malloc(sizeof(vector));
  vector q_0, q_1;
  double alpha = 1.0, beta = 0.0;
  
  vector_init(orth_poly, n, sizeof(vector));
  vector_init(&q_0, 1, sizeof(double));
  vector_init(&q_1, 2, sizeof(double));

  vector_push(&q_0, &alpha); 
  
  for (size_t i = 0; i < E->rows; ++i) {
    beta += matrix_val(E, i, 1);
  }
  beta /= -(double)E->rows;
  vector_push(&q_1, &beta);
  vector_push(&q_1, &alpha);
  
  vector_push(orth_poly, &q_0);
  vector_push(orth_poly, &q_1);
  return orth_poly;
}


vector* orth_step(size_t n, matrix *E, vector *f, vector *orth_poly) {
  static vector x_vec;
  double alpha = 1.0, beta = 0.0;
  if (x_vec.size == 0) {
    vector_init(&x_vec, 2, sizeof(double));
    vector_push(&x_vec, &beta);
    vector_push(&x_vec, &alpha);
  }
  
  for (size_t i = orth_poly->size; orth_poly->size < n; ++i) {
    vector tmp_2, tmp_3;
    double sum_uppr = 0.0, sum_bott= 0.0, sum_uppr_b = 0.0, sum_bott_b = 0.0;
    
    vector_init_copy(&tmp_2, vector_get(orth_poly, i - 1));
    vector_init_copy(&tmp_3, vector_get(orth_poly, i - 2));
    vector *tmp = poly_mult(&x_vec, vector_get(orth_poly, i - 1));
    
    for (size_t j = 0; j < E->rows; ++j) {
      sum_uppr += matrix_val(E, j, 1) * pow(poly_val(vector_get(orth_poly, i - 1), matrix_val(E, j, 1)), 2.0);
      sum_bott += pow(poly_val(vector_get(orth_poly, i - 1), matrix_val(E, j, 1)), 2.0);
      sum_uppr_b += matrix_val(E, j, 1) * poly_val(vector_get(orth_poly, i - 1), matrix_val(E, j, 1)) *
        poly_val(vector_get(orth_poly, i - 2), matrix_val(E, j, 1));
      sum_bott_b += pow(poly_val(vector_get(orth_poly, i - 2), matrix_val(E, j, 1)), 2.0);
    }
    alpha = -sum_uppr/sum_bott;
    beta = -sum_uppr_b/sum_bott_b;
    vector_mult(&tmp_2, alpha);
    vector_mult(&tmp_3, beta);
    
    poly_sum(tmp, &tmp_2);
    poly_sum(tmp, &tmp_3);
    vector_push(orth_poly, tmp);
    
    vector_free(&tmp_2);
    vector_free(&tmp_3);
    free(tmp);
  }
  
  for (size_t i = 0; i < orth_poly->size; ++i) {
    double sum_uppr = 0.0, sum_bott = 0.0;
    for (size_t j = 0; j < E->rows; ++j) {
      sum_uppr += poly_val(vector_get(orth_poly, i), matrix_val(E, j, 1)) * vector_val(f, j);
      sum_bott += pow(poly_val(vector_get(orth_poly, i), matrix_val(E, j, 1)), 2.0);
    }
    vector_mult(vector_get(orth_poly, i), sum_uppr/sum_bott);
  }

  vector *res = (vector *)malloc(sizeof(vector));
  vector_init_copy(res, vector_get(orth_poly, orth_poly->size - 1));

  for (size_t i = 0; i < orth_poly->size - 1; ++i) {
    poly_sum(res, vector_get(orth_poly, i));
  }
  
  vector_free(&x_vec);

  return res;
}

int main(void) {
  char *or_func = "x^2 tan(x)";
  str_func(or_func);

  FILE *csv = fopen("out.csv", "w");
  if (!csv) return -1;
  fprintf(csv, "n, sigma_ne, sigma_ore\n");
  
  /*  Search best and make a table */
  pair min_a = {1.0, 0.0}, min_b = {1.0, 0.0};
  for (size_t n = 10; n < 200; ++n) {
    vector *a = entry(n, M_POINTS, 0);
    vector *b = entry(n, M_POINTS, 1);
    
    double add = (BOUND_B - BOUND_A) / CONTROL_POINTS;
    double x = -1;

    double sum_a = 0.0, sum_b = 0.0;
    for (size_t i = 0; i < CONTROL_POINTS; ++i, x += add) {
      sum_a += pow((poly_val(a, x) - func(x)), 2);
      sum_b += pow((poly_val(b, x) - func(x)), 2);
    }

    min_a = sum_a < min_a.a ? (pair){sum_a, n} : min_a;
    min_b = sum_b < min_b.a ? (pair){sum_b, n} : min_b;

    fprintf(csv, "%zu, %lf, %lf\n", n, sum_a, sum_b);
    
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

  vector *best_ne = entry((size_t)min_a.b, M_POINTS, 0);
  vector *best_ore = entry((size_t)min_b.b, M_POINTS, 1);
  printf("\n%zu %zu\n", (size_t)min_a.b, (size_t)min_b.b);
  
  add_func(best_ne);
  add_func(best_ore);
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

  str_func(or_func);  

  size_t orth_size = orth->size;
  for (size_t i = 5; i >= 1; --i, orth->size--) {
    vector *tmp_a = entry(i, M_POINTS, 0);
    vector *tmp_b = entry(i, M_POINTS, 1);

    add_func(tmp_a);
    add_func(tmp_b);
    
    vector_free(tmp_a);
    vector_free(tmp_b);
    free(tmp_a);
    free(tmp_b);
  }
  orth->size = orth_size;
  
  plot();
  clear_plot();
  
  for (size_t i = orth->size - 1; i > 1; --i) {
    vector_free(vector_get(orth, i));
    free(vector_get(orth, i));
    orth->size--;
  }  
  vector_free(orth);

  return 0;
}

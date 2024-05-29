#include <stdio.h>
#include <jemalloc/jemalloc.h>

#include "../types/vector.h"
#include "../types/matrix.h"
#include "../types/pair.h"
#include "polynoms.h"
#include "../common.h"
#include "gauss.h"

/* :) */
double inline get_hi(vector *points, size_t i) {
  return pair_get(points, i + 1).a - pair_get(points, i).a;
}

vector* qube_spline(vector *points) {
  vector *res = (vector *)malloc(sizeof(vector));
  vector_init(res, points->size - 1, sizeof(vector));

  matrix H;
  matrix_init(&H, points->size - 2, points->size - 2, sizeof(double));
  matrix_fill_smth(&H, 0.0);
  
  
  matrix_val(&H, 0, 0) = 2 * (get_hi(points, 0)
                            + get_hi(points, 0 + 1));
  matrix_val(&H, 0 + 1, 0 + 1) = 2 * (get_hi(points, 1)
                                    + get_hi(points, 1 + 1));
  matrix_val(&H, 0, 0 + 1) = get_hi(points, 0 + 1);
  matrix_val(&H, 0 + 1, 0) = matrix_val(&H, 0, 0 + 1);
  
  vector y;
  vector_init(&y, points->size - 2, sizeof(double));
  vector_fill_smth(&y, 0.0);  
  
  for (size_t i = 0; i < points->size - 2; ++i) {
    vector_val(&y, i) =
      6 * (pair_get(points, i + 2).b - pair_get(points, i + 1).b)
      / get_hi(points, i + 1);
    vector_val(&y, i) -=
      6 * (pair_get(points, i + 1).b - pair_get(points, i).b)
      / get_hi(points, i);
  }
  
  vector *uay = gauss(&H, &y);

  double natural_spline = 0;
  vector_push(uay, &natural_spline);
  vector_reverse(uay);
  vector_push(uay, &natural_spline);
  vector_reverse(uay);
  
  for (size_t i = 0; i < uay->size - 1; ++i) {
    double y_i_strh = (pair_get(points, i + 1).b - pair_get(points, i).b) / get_hi(points, i);
    y_i_strh -= (vector_val(uay, i + 1) * get_hi(points, i) / 6);
    y_i_strh -= (vector_val(uay, i) * get_hi(points, i) / 3);

    vector *app = (vector *)malloc(sizeof(vector));
    vector_init(app, 4, sizeof(double));
    vector_fill_smth(app, 0.0);
  
    vector_val(app, 0) = pair_get(points, i).b;

    vector xminus, xminus2;
    vector_init(&xminus, 2, sizeof(double));
    
    vector_fill_smth(&xminus, 1.0);
    vector_val(&xminus, 0) = -pair_get(points, i).a;
    vector_init_copy(&xminus2, &xminus);

    vector_mult(&xminus, y_i_strh);
    poly_sum(app, &xminus);

    vector_assign(&xminus, &xminus2);
    vector mult = poly_mult(&xminus, &xminus2);
    vector_mult(&mult, vector_val(uay, i) / 2);
    poly_sum(app, &mult);
    vector_free(&mult);

    mult = poly_mult(&xminus, &xminus2);
    vector mult_c = poly_mult(&mult, &xminus);
    vector_mult(&mult_c, (vector_val(uay, i + 1) - vector_val(uay, i)) / (6 * get_hi(points, i)));
    poly_sum(app, &mult_c);

    vector_push(res, app);

    vector_free(&mult_c);
    vector_free(&mult);
    vector_free(&xminus);
    vector_free(&xminus2);
  }
  
  vector_free(uay);
  free(uay);
  
  matrix_free(&H);
  vector_free(&y);
  
  return res;
}

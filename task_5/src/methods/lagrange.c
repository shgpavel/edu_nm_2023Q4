#include <jemalloc/jemalloc.h>
#include <math.h>
#include <stdio.h>

#include "../common.h"
#include "../types/pair.h"
#include "../types/vector.h"
#include "polynoms.h"

vector *lagrange_poly(vector *points) {
  vector *result = (vector *)malloc(sizeof(vector));
  vector v;
  vector tmp;

  vector_init(result, points->size, sizeof(double));
  vector_init(&tmp, 2, sizeof(double));
  vector_init(&v, 2, sizeof(double));

  vector_fill_smth(result, 0.0);
  vector_fill_smth(&tmp, 0.0);

  double tmp_num = 1.0;
  vector_change(&tmp, 1, &tmp_num);

  for (size_t i = 0; i < points->size; ++i) {
    for (size_t j = 0; j < points->size; ++j) {
      if (j == i) continue;

      tmp_num = -(pair_get(points, j).a);
      vector_change(&tmp, 0, &tmp_num);

      if (j == 0 || (j == 1 && i == 0)) {
        vector_assign(&v, &tmp);
      } else {
        vector v_next = poly_mult(&v, &tmp);
        vector_swap_eff(&v, &v_next);
        vector_free(&v_next);
      }
    }

    for (size_t j = 0; j < points->size; ++j) {
      if (j != i) {
        double denom = pair_get(points, i).a - pair_get(points, j).a;
        if (fabs(denom) > divtol) {
          vector_mult(&v, 1 / denom);
        }
      }
    }
    vector_mult(&v, pair_get(points, i).b);
    poly_sum(result, &v);
  }

  vector_free(&tmp);
  vector_free(&v);
  return result;
}

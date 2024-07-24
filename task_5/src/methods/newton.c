#include <jemalloc/jemalloc.h>
#include <math.h>
#include <stdio.h>

#include "../common.h"
#include "../funcs/funcs.h"
#include "../types/pair.h"
#include "../types/vector.h"
#include "polynoms.h"

vector *newton_poly(vector *points) {
  vector *res = (vector *)malloc(sizeof(vector));
  vector_init(res, points->size, sizeof(double));

  vector xpolres;
  vector xminus;

  vector_init(&xpolres, 2, sizeof(double));
  vector_init(&xminus, 2, sizeof(double));

  vector_fill_smth(res, 0.0);
  vector_fill_smth(&xminus, 1.0);

  for (size_t k = 0; k < points->size; ++k) {
    double g = 1;
    double f = 0;
    for (size_t i = 0; i <= k; ++i) {
      g = 1;
      for (size_t j = 0; j <= k; ++j) {
        if (i != j) {
          double denom = pair_get(points, i).a - pair_get(points, j).a;
          if (fabs(denom) > divtol) g /= denom;
        }
      }
      f += g * pair_get(points, i).b;
    }

    for (size_t i = 0; i < k; ++i) {
      vector_val(&xminus, 0) = -(pair_get(points, i).a);
      if (i != 0) {
        vector xpolres_next = poly_mult(&xpolres, &xminus);
        vector_swap_eff(&xpolres, &xpolres_next);
        vector_free(&xpolres_next);
      } else {
        vector_assign(&xpolres, &xminus);
      }
    }
    vector_mult(&xpolres, f);
    poly_sum(res, &xpolres);
  }

  vector_val(&xminus, 0) = pair_get(points, 0).b;
  vector_val(&xminus, 1) = 0;
  poly_sum(res, &xminus);

  vector_free(&xminus);
  vector_free(&xpolres);

  return res;
}

/* LEN' but */
void newton_step(vector *res, vector *points);

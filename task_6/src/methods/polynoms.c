#include <stdio.h>
#include <complex.h>
#include <math.h>
#include <jemalloc/jemalloc.h>

#include "../common.h"
#include "../types/vector.h"

void fft(complex double *x, size_t n, size_t inverse) {
  if (n <= 1) return;

  complex double even[n / 2];
  complex double odd[n / 2];
  for (size_t i = 0; i < n / 2; i++) {
    even[i] = x[2 * i];
    odd[i] = x[2 * i + 1];
  }

  fft(even, n / 2, inverse);
  fft(odd, n / 2, inverse);

  complex double W_N = cexp((2 * M_PI * I) / n * (inverse ? 1 : -1));
  complex double W = 1;
  for (size_t k = 0; k < n / 2; k++) {
    complex double t = W * odd[k];
    x[k] = even[k] + t;
    x[k + n / 2] = even[k] - t;
    W *= W_N;
  }
}

vector* poly_mult(vector *poly_1, vector *poly_2) {
  vector *result = (vector *)malloc(sizeof(vector));
  vector_init(result, poly_1->size, sizeof(double));

  size_t result_deg = poly_1->size + poly_2->size - 1, n = 1;
  while (n < result_deg) {
    n *= 2;
  }

  complex double *fft_poly_1 = (complex double *) calloc(n, sizeof(complex double));
  complex double *fft_poly_2 = (complex double *) calloc(n, sizeof(complex double));
  for (size_t i = 0; i < poly_1->size; ++i) {
    fft_poly_1[i] = vector_val(poly_1, i);
  }
  for (size_t i = 0; i < poly_2->size; ++i) {
    fft_poly_2[i] = vector_val(poly_2, i);
  }


  fft(fft_poly_1, n, 0);
  fft(fft_poly_2, n, 0);
  for (size_t i = 0; i < n; i++) {
    fft_poly_1[i] *= fft_poly_2[i];
  }

  fft(fft_poly_1, n, 1);
  for (size_t i = 0; i < result_deg; ++i) {
    double tmp = (creal(fft_poly_1[i]) / n);
    vector_push(result, &tmp);
  }

  free(fft_poly_1);
  free(fft_poly_2);
  return result;
}

void poly_sum(vector *v, vector *d) {
  if (v->type_size == d->type_size) {
    size_t min_size = (v->size < d->size) ? v->size : d->size;
    for (size_t i = 0; i < min_size; ++i) {
      vector_val(v, i) += vector_val(d, i);
    }
  }
}

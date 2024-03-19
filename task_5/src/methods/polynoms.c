#include <stdio.h>
#include <complex.h>
#include <math.h>
#include <jemalloc/jemalloc.h>

#include "../types/vector.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef unwrap_double
#define unwrap_double(ptr) (*(double *) (ptr))
#endif

void fft(complex double *x, size_t N, size_t inverse) {
    if (N <= 1) return;

    complex double even[N / 2];
    complex double odd[N / 2];
    for (size_t i = 0; i < N / 2; i++) {
        even[i] = x[2 * i];
        odd[i] = x[2 * i + 1];
    }

    fft(even, N / 2, inverse);
    fft(odd, N / 2, inverse);

    complex double W_N = cexp((2 * M_PI * I) / N * (inverse ? 1 : -1));
    complex double W = 1;
    for (size_t k = 0; k < N / 2; k++) {
        complex double t = W * odd[k];
        x[k] = even[k] + t;
        x[k + N / 2] = even[k] - t;
        W *= W_N;
    }
}

vector poly_mult(vector *poly_1, vector *poly_2) {
  vector result;
  vector_init(&result, poly_1->size, sizeof(double));

  size_t result_deg = poly_1->size + poly_2->size - 1, n = 1;
  while (n < result_deg) {
    n *= 2;
  }

  complex double *fft_poly_1 = (complex double *) calloc(n, sizeof(complex double));
  complex double *fft_poly_2 = (complex double *) calloc(n, sizeof(complex double));
  
  for (size_t i = 0; i < poly_1->size; ++i) {
    fft_poly_1[i] = unwrap_double(vector_get(poly_1, i));
  }
  
  for (size_t i = 0; i < poly_2->size; ++i) {
    fft_poly_2[i] = unwrap_double(vector_get(poly_2, i));
  }


  fft(fft_poly_1, n, 0);
  fft(fft_poly_2, n, 0);
  
  for (size_t i = 0; i < n; i++) {
    fft_poly_1[i] *= fft_poly_2[i];
  }

  fft(fft_poly_1, n, 1);
  
  for (size_t i = 0; i < result_deg; ++i) {
    double tmp = (creal(fft_poly_1[i]) / n);
    vector_push(&result, &tmp);
  }

  free(fft_poly_1);
  free(fft_poly_2);
  
  return result;
}

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <jemalloc/jemalloc.h>


#include "../common.h"
#include "../types/vector.h"
#include "../types/eigenpair.h"
#include "../types/matrix.h"
#include "solv.h"

#define MAX_ITER 100
#define MAX_TIME 1000000000

/*

I dont really know its normal or not that
this algo is absolute trash or I just missed
something out

UPD: .......... just tried to make it work :?
It still does not)) dont use it please

If someone somehow will figure the problem in this code
please contact me at shgpavel@yandex.ru

*/


inline matrix prep_matrix(matrix *a, double se) {
  matrix res;
  matrix_init_copy(&res, a);
  for (size_t i = 0; i < a->rows; ++i) {
    unwrap_double(matrix_get(&res, i, i)) -= se;
  }
  return res;
}

vector* inverse_iter_flex_sigm(matrix *m) {
  matrix a;
  matrix_init_copy(&a, m);
  
  vector *all_pairs = (vector *)malloc(sizeof(vector));
  vector_init(all_pairs, a.rows, sizeof(eigenpair));
  
  for (double sigma = -matrix_norm_inf(&a);
       all_pairs->size < a.rows &&
       sigma < (matrix_norm_inf(&a) + 1e-2);
       sigma += 1e-2) {
    eigenpair tmp_ep;
    vector next_vec, cur_vec;
    double sigma = BOUND_A;

	  vector_init(&cur_vec, a.rows, sizeof(double));
    vector_init(&next_vec, a.rows, sizeof(double));
  
    vector_fill_smth(&cur_vec, sigma);
    vector_fill_smth(&next_vec, sigma);
    vector_normalize(&cur_vec);
  
    size_t flag = 1;
    for (size_t j = 0; j < MAX_ITER; ++j) {
      matrix tmp_m = prep_matrix(&a, sigma);
      vector *tmp_v = gauss(&tmp_m, &cur_vec);
      matrix_free(&tmp_m);
      vector_free(&next_vec);
      vector_copy_from_heap(&next_vec, tmp_v);

      double tmp = 0;
      tmp_ep.eigenvalue = 0.0;
      for (size_t i = 0; i < a.rows; ++i) {
			  if (unwrap_double(vector_get(&next_vec, i)) < delta_c) continue;
		    tmp = (unwrap_double(vector_get(&cur_vec, i)) /
										          unwrap_double(vector_get(&next_vec, i)));
			  tmp_ep.eigenvalue += tmp;
		  }
      tmp_ep.eigenvalue /= ((double)a.rows);
      sigma += tmp_ep.eigenvalue;
      
      vector_normalize(&next_vec);

      flag = 1;
      for (size_t i = 0; i < a.rows; ++i) {
        if (fabs(tmp_ep.eigenvalue) > rtol) flag = 0;
        if (fabs(unwrap_double(vector_get(&next_vec, i)) -
               unwrap_double(vector_get(&cur_vec, i))) > rtol) flag = 0;
      }
      if (flag == 1) break;
 
      vector_swap_eff(&cur_vec, &next_vec);
    }
  
    vector_swap_eff(&tmp_ep.eigenvector, &next_vec);
    tmp_ep.eigenvalue = sigma;

    if (flag == 1) {
      vector_push(all_pairs, &tmp_ep);
    }
    vector_free(&cur_vec);
  }
  matrix_free(&a);

  return all_pairs;
}

inline void deflate(matrix *a, eigenpair *ep) {
  for (size_t i = 0; i < a->rows; ++i) {
    for (size_t j = 0; j < a->rows; ++j) {
      unwrap_double(matrix_get(a, i, j)) -= ep->eigenvalue *
        unwrap_double(vector_get(&ep->eigenvector, i)) *
        unwrap_double(vector_get(&ep->eigenvector, j));
    }
  }
}

vector* inverse_iter_reley(matrix *m) {
  matrix a;
  matrix_init_copy(&a, m);
  
  vector *all_pairs = (vector *)malloc(sizeof(vector));
  vector_init(all_pairs, a.rows, sizeof(eigenpair));

  while (all_pairs->size < a.rows) { 
    double sigma = BOUND_A;
    eigenpair tmp_ep;

    vector next_vec;
    vector cur_vec;
	  vector_init(&cur_vec, a.rows, sizeof(double));
    vector_init(&next_vec, a.rows, sizeof(double));
  
    vector_fill_smth(&cur_vec, 1.0);
    vector_fill_smth(&next_vec, sigma);

    vector_normalize(&cur_vec);

    size_t flag;
    for (size_t j = 0; j < MAX_ITER; ++j) {
      vector *tmp_v = matrix_on_vector(&a, &cur_vec);
      sigma = vector_sclr_prod(tmp_v, &cur_vec) *
              vector_sclr_prod(&cur_vec, &cur_vec); 
      vector_free(tmp_v);
      free(tmp_v);
    
      matrix tmp_m = prep_matrix(&a, sigma);
      tmp_v = gauss(&tmp_m, &cur_vec);
      vector_free(&next_vec);
      matrix_free(&tmp_m);
      vector_copy_from_heap(&next_vec, tmp_v);
      vector_normalize(&next_vec);

      flag = 1;
      for (size_t i = 0; i < a.rows; ++i) {
        if (fabs(unwrap_double(vector_get(&next_vec, i)) -
                unwrap_double(vector_get(&cur_vec, i))) > rtol) flag = 0;
      }
      if (flag == 1) break;

      vector_swap_eff(&cur_vec, &next_vec);
    }
  
    if (flag == 1) {
      vector_swap_eff(&tmp_ep.eigenvector, &next_vec);
      tmp_ep.eigenvalue = sigma;
      
      deflate(&a, &tmp_ep);
      vector_push(all_pairs, &tmp_ep);
    }
    
    vector_free(&cur_vec);
  }
  matrix_free(&a);

  return all_pairs;
}

void* thread_func_iifs(void* arg) {
  matrix *a = (matrix *)arg;
  vector *tmp = inverse_iter_flex_sigm(a);
  return tmp; 
}

void* thread_func_iir(void* arg) {
  matrix *a = (matrix *)arg;
  vector *tmp = inverse_iter_reley(a);
  return tmp;
}

void normalize_timespec(struct timespec *ts) {
  while (ts->tv_nsec >= MAX_TIME) {
    ts->tv_nsec -= MAX_TIME;
    ts->tv_sec++;
  }
  while (ts->tv_nsec < 0) {
    ts->tv_nsec += MAX_TIME;
    ts->tv_sec--;
  }
}

vector* inverse_iter(matrix *a) {
  
  pthread_t thread_one;
  pthread_t thread_two;

  vector *out;

  if (pthread_create(&thread_one, NULL, thread_func_iir, a) != 0) {
    fprintf(stderr, "Error| Failed to create thread_one\n");
    exit(1);
  }
  
  struct timespec ts;
  clock_gettime(CLOCK_REALTIME, &ts);
  ts.tv_sec += 10;
  normalize_timespec(&ts);

  int res = pthread_timedjoin_np(thread_one, (void**)&out, &ts);
  if (res == ETIMEDOUT) {
    printf("Log|  Switching from reley/hangs\n");
    pthread_cancel(thread_one);
  } else if (res == 0) {
    printf("Log|  Result\n");
    return out;
  }
  
  if (pthread_create(&thread_two, NULL, thread_func_iifs, a) != 0) {
    fprintf(stderr, "Error| Failed to create thread_two\n");
    exit(1);
  }

  clock_gettime(CLOCK_REALTIME, &ts);
  ts.tv_sec += 10;
  normalize_timespec(&ts);

  res = pthread_timedjoin_np(thread_two, (void**)&out, &ts);
  if (res == ETIMEDOUT) {
    printf("Log|  Switching from shifts/hangs\n");
    pthread_cancel(thread_two);
  } else if (res == 0) {
    printf("Log|  Result\n");
    return out;
  }
  
  fprintf(stderr, "Error| Nothing worked so inv iter interrupted\n");
  exit(1);
}

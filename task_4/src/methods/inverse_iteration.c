#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include <time.h>
#include <errno.h>
#include <jemalloc/jemalloc.h>

#include "../common.h"
#include "../types/vector.h"
#include "../types/eigenpair.h"
#include "../types/matrix.h"
#include "gauss.h"
#include "rng.h"

#define MAX_ITER 10000
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

struct args {
  int joined;
  pthread_t td;
  pthread_mutex_t mtx;
  pthread_cond_t cond;
  void **res;
};

static void *waiter(void *ap) {
  struct args *args = ap;
  pthread_join(args->td, args->res);
  pthread_mutex_lock(&args->mtx);
  args->joined = 1;
  pthread_mutex_unlock(&args->mtx);
  pthread_cond_signal(&args->cond);
  return 0;
}

int pthread_timedjoin_np(pthread_t td, void **res, struct timespec *ts) {
  pthread_t tmp;
  int ret;
  struct args args = { .td = td, .res = res };

  pthread_mutex_init(&args.mtx, 0);
  pthread_cond_init(&args.cond, 0);
  pthread_mutex_lock(&args.mtx);

  ret = pthread_create(&tmp, 0, waiter, &args);
  if (!ret)
    do ret = pthread_cond_timedwait(&args.cond, &args.mtx, ts);
  while (!args.joined && ret != ETIMEDOUT);

  pthread_mutex_unlock(&args.mtx);

  pthread_cancel(tmp);
  pthread_join(tmp, 0);

  pthread_cond_destroy(&args.cond);
  pthread_mutex_destroy(&args.mtx);
  return args.joined ? 0 : ret;
}

inline matrix prep_matrix(matrix *a, double se) {
  matrix res;
  matrix_init_copy(&res, a);
  for (size_t i = 0; i < a->rows; ++i) {
    matrix_val(&res, i, i) -= se;
  }
  return res;
}

inline void deflate(matrix *a, eigenpair *ep) {
  for (size_t i = 0; i < a->rows; ++i) {
    for (size_t j = 0; j < a->rows; ++j) {
      matrix_val(a, i, j) -= ep->eigenvalue * 
        vector_val(&ep->eigenvector, i) *
        vector_val(&ep->eigenvector, j);
    }
  }
}

vector* inverse_iter_flex_sigm(matrix *m) {
  matrix a;
  matrix_init_copy(&a, m);

  vector *all_pairs = (vector *)malloc(sizeof(vector));
  vector_init(all_pairs, a.rows, sizeof(eigenpair));

  for (double sigma = -matrix_norm_inf(&a) - 1e-2;
       sigma < (matrix_norm_inf(&a) + 1e-2);
       sigma += rtol * rtol) {
    eigenpair tmp_ep;
    vector next_vec, cur_vec;

	  vector_init(&cur_vec, a.rows, sizeof(double));
    vector_init(&next_vec, a.rows, sizeof(double));

    vector_fill_smth(&cur_vec, INITIAL);
    vector_fill_smth(&next_vec, INITIAL);
    vector_normalize(&cur_vec);

    size_t flag = 1;
    for (size_t j = 0; j < MAX_ITER; ++j) {
      matrix tmp_m = prep_matrix(&a, sigma);
      vector *tmp_v = gauss(&tmp_m, &cur_vec);
      matrix_free(&tmp_m);
      vector_free(&next_vec);
      vector_from_heap_to_stack(&next_vec, tmp_v);

      double tmp = 0;
      tmp_ep.eigenvalue = 0.0;
      for (size_t i = 0; i < a.rows; ++i) {
        if (fabs(vector_val(&next_vec, i)) < delta_c) continue;
        tmp = (vector_val(&cur_vec, i) / vector_val(&next_vec, i));
		    tmp_ep.eigenvalue += tmp;
	    }
      tmp_ep.eigenvalue /= ((double)a.rows);
      sigma += tmp_ep.eigenvalue;

      vector_normalize(&next_vec);

      flag = 1;
      for (size_t i = 0; i < a.rows; ++i) {
        if (fabs(tmp_ep.eigenvalue) > rtol) flag = 0;
        if (fabs(vector_val(&next_vec, i) -
               vector_val(&cur_vec, i)) > rtol) flag = 0;
      }
      if (flag == 1) break;

      vector_swap_eff(&cur_vec, &next_vec);
    }

    if (flag == 1 && sigma < BOUND_B && sigma > BOUND_A && fabs(sigma) > 1e-3) {
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

vector* inverse_iter_reley(matrix *m) {
  matrix a;
  matrix_init_copy(&a, m);

  vector *all_pairs = (vector *)malloc(sizeof(vector));
  vector_init(all_pairs, a.rows, sizeof(eigenpair));

  for (size_t i = 0; i < MAX_ITER; ++i) {
    double sigma;
    eigenpair tmp_ep;

    vector next_vec;
    vector cur_vec;
	  vector_init(&cur_vec, a.rows, sizeof(double));
    vector_init(&next_vec, a.rows, sizeof(double));

    vector_fill_smth(&cur_vec, INITIAL);
    vector_fill_smth(&next_vec, INITIAL);

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
      vector_from_heap_to_stack(&next_vec, tmp_v);
      vector_normalize(&next_vec);

      flag = 1;
      for (size_t i = 0; i < a.rows; ++i) {
        if (fabs(vector_val(&next_vec, i) -
              vector_val(&cur_vec, i)) > rtol) flag = 0;
      }
      if (flag == 1) break;
      vector_swap_eff(&cur_vec, &next_vec);
    }

    if (flag == 1 && sigma < BOUND_B && sigma > BOUND_A && fabs(sigma) > 1e-3) {
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

vector* concat_res(vector *a, vector *b, matrix *c) {
  vector *result = (vector *)malloc(sizeof(vector));
  vector_init(result, c->rows, sizeof(eigenpair));

  vector_swap_eff(result, a);
  free(a);

  for (size_t i = 0; i < b->size /* && i < c->rows */; ++i) {
    size_t seen = 0;
    for (size_t j = 0; j < result->size; ++j) {
      if (fabs(
            ((eigenpair *)vector_get(result, j))->eigenvalue -
            ((eigenpair *)vector_get(b, i))->eigenvalue) < rtol) seen = 1; 
    }
    if (!seen) {
      vector_push(result, vector_get(b, i));
    }
  }

  vector_free(b);
  free(b);
  return result;
}

vector* inverse_iter(matrix *a) {
  pthread_t thread_one;
  pthread_t thread_two;

  vector *out_rel;
  vector *out_shf;

  if (pthread_create(&thread_one, NULL, thread_func_iir, a) != 0) {
    fprintf(stderr, "Error| Failed to create thread_one\n");
    return NULL;
  }

  struct timespec ts;
  clock_gettime(CLOCK_REALTIME, &ts);
  ts.tv_sec += 10;

  int res = pthread_timedjoin_np(thread_one, (void**)&out_rel, &ts);
  
  if (res == ETIMEDOUT) {
    printf("[Log]  Switching from reley|hangs\n");
    pthread_cancel(thread_one);
    out_rel = NULL;
  } else if (res == 0) {
    printf("[Log]  Result inv iter (reley)\n");
  }

  if (pthread_create(&thread_two, NULL, thread_func_iifs, a) != 0) {
    fprintf(stderr, "[Error] Failed to create thread_two\n");
    return NULL;
  }

  clock_gettime(CLOCK_REALTIME, &ts);
  ts.tv_sec += 10;

  res = pthread_timedjoin_np(thread_two, (void**)&out_shf, &ts);
  if (res == ETIMEDOUT) {
    printf("[Log]  Switching from shifts|hangs\n");
    pthread_cancel(thread_two);
    out_shf = NULL;
  } else if (res == 0) {
    printf("[Log]  Result inv iter (shifts)\n");
  }

  if (out_rel == NULL && out_shf == NULL) {
    fprintf(stderr, "[Error] Nothing worked so inv iter interrupted\n");
    return NULL;
  }
  else if (out_rel == NULL) return out_shf;
  else if (out_shf == NULL) return out_rel;
  else {
    vector *result = concat_res(out_rel, out_shf, a);
    return result;
  }
}

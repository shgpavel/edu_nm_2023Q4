#ifndef VECTOR_H
#define VECTOR_H


#include <stddef.h>

typedef struct vector_s {
  size_t size;
  size_t capacity;
  size_t type_size;
  void **data;
} vector;

void   swap_xor_st(size_t *, size_t *);
int    on_heap(void *);

void   vector_init(vector *, size_t, size_t);
void   vector_init_copy(vector *, vector *);
void   vector_push(vector *, void *);
void   vector_change(vector *, size_t, void *);
void   vector_assign(vector *, vector *);
void   vector_swap(vector *, size_t, size_t);
void*  vector_get(vector *, size_t);
void   vector_delete(vector *, size_t);
void   vector_free(vector *);
void   vector_from_heap_to_stack(vector *, vector *);
void   vector_swap_eff(vector *, vector *);
void   vector_print(vector *);
void   vector_fill_smth(vector *, double);
void   vector_mult(vector *v, double a);
double vector_diff(vector *, vector *);
double vector_sclr_prod(vector *, vector *);
void   vector_normalize(vector *);

#endif

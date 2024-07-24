#ifndef VECTOR_H
#define VECTOR_H

typedef struct vector_s {
  size_t size;
  size_t capacity;
  size_t type_size;
  void **data;
} vector;

void vector_init(vector *, size_t, size_t);
void vector_init_copy(vector *, vector *);
void vector_push(vector *, void *);
void vector_fill_zero(vector *);
void vector_swap(vector *, size_t, size_t);
void vector_change(vector *, size_t, void *);
void vector_mult(vector *v, double a);
double vector_diff(vector *, vector *);
void vector_assign(vector *, vector *);
void vector_delete(vector *, size_t);
void *vector_get(vector *, size_t);
void vector_print(vector *);
void vector_print_pairs(vector *);
void vector_free(vector *);

#endif

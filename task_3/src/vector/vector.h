#ifndef VECTOR_H_
#define VECTOR_H_


typedef struct vector_s {
    void **data;
    size_t size;
    size_t capacity;
    size_t type_size;
} vector;


void   vector_init(vector *, size_t, size_t);
void   vector_init_copy(vector *dest, vector *src);
void   vector_push(vector *, void *);
void   vector_fill_zero(vector *v);
void   vector_swap(vector *v, size_t i, size_t j);
void   vector_change(vector *, size_t, void *);
double vector_diff(vector *x, vector *y);
void   vector_assign(vector *v, vector *c);
void   vector_delete(vector *, size_t);
void*  vector_get(vector *, size_t);
void   vector_print(vector *v);
void   vector_free(vector *);


#endif

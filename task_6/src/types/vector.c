#include <stdio.h>
#include <string.h>
#include <math.h>
#include <jemalloc/jemalloc.h>

#include "vector.h"
#include "../common.h"


int on_heap(void *p) {
  int stack_item;
  return p < (void *)&stack_item;
}

void swap_xor_st(size_t *a, size_t *b) {
  *a ^= *b;
  *b ^= *a;
  *a ^= *b;
}

void vector_init(vector *v, size_t capacity, size_t size_type) {
  v->data = (void **) malloc(capacity * sizeof(void *));
  v->size = 0;
  v->capacity = capacity;
  v->type_size = size_type;
}

void vector_init_copy(vector *dest, vector *src) {
  dest->data = (void **) malloc(src->capacity * sizeof(void *));
  dest->size = src->size;
  dest->capacity = src->capacity;
  dest->type_size = src->type_size;
  for (size_t i = 0; i < src->size; ++i) {
    dest->data[i] = (void *) malloc(src->type_size);
    memcpy(dest->data[i], src->data[i], src->type_size);
  }
}

void vector_resize(vector *v, size_t new_capacity) {
  v->capacity = new_capacity;
  v->data = realloc(v->data, new_capacity * sizeof(void *));
}

void vector_push(vector *v, void *atad) {
  if (v->size >= v->capacity) {
	  vector_resize(v, v->capacity + v->capacity / 2);
  }

  v->data[v->size] = (void *) malloc(v->type_size);
  memcpy(v->data[v->size], atad, v->type_size);
  v->size++;
}

void vector_change(vector *v, size_t index, void *atad) {
  if (index < v->size) {
	  memcpy(v->data[index], atad, v->type_size);
  }
}

void vector_assign(vector *v, vector *c) {
	if (v->size > c->size) {
    vector_free(v);
    vector_init_copy(v, c);
  } else {

  for (size_t i = 0; i < v->size; ++i) {
	  vector_change(v, i, vector_get(c, i));
	}

  if (v->size < c->size) {
    for (size_t i = v->size; i < c->size; ++i) {
      vector_push(v, vector_get(c, i));
    }
  }}
}

void vector_swap(vector *v, size_t i, size_t j) {
  if (i < v->size && j < v->size) {
	  void *tmp = (void *) malloc(v->type_size);
	  memcpy(tmp, v->data[i], v->type_size);
	  memcpy(v->data[i], v->data[j], v->type_size);
	  memcpy(v->data[j], tmp, v->type_size);
	  free(tmp);
  }
}

void* vector_get(vector *v, size_t index) {
  if (index < v->size) {
	  return v->data[index];
  }
  return NULL;
}

void vector_delete(vector *v, size_t index) {
  if (index < v->size - 1) {
	  free(vector_get(v, index));
	  for (size_t i = index; i < v->size; ++i) {
	    v->data[i] = v->data[i + 1];
	  }
	  v->size--;
  } else if (index == v->size - 2){
    free(vector_get(v, index));
    v->size--;
  }
}

void vector_free(vector *v) {
  for (size_t i = 0; i < v->size; ++i) {
	  free(v->data[i]);
  }

  v->size = 0;
  v->capacity = 0;
  free(v->data);
}

void vector_from_heap_to_stack(vector *v, vector *c) {
  *v = *c;
  c->data = NULL;
  if (on_heap(c)) {
    free(c);
  }
}

void vector_swap_eff(vector *v, vector *c) {
  swap_xor_st(&v->size, &c->size);
  swap_xor_st(&v->capacity, &c->capacity);
  swap_xor_st(&v->type_size, &c->type_size);

  void **tmp = v->data;
  v->data = c->data;
  c->data = tmp;
}

void vector_print(vector *v) {
  if (v != NULL && v->type_size == sizeof(double)) {
	  for (size_t i = 0; i < v->size; ++i) {
	    printf("%g ", vector_val(v, i));
    }
	  printf("\n");
  }
}

void vector_fill_smth(vector *v, double x) {
  if (v->type_size == sizeof(double)) {
	  for (size_t i = 0; i < v->capacity; ++i) {
	    double tmp = x;
	    vector_push(v, (void *)&tmp);
	  }
  }
}

void vector_mult(vector *v, double a) {
  if (v->type_size == sizeof(double)) {
    for (size_t i = 0; i < v->size; ++i) {
      vector_val(v, i) *= a;
    }
  }
}

double vector_diff(vector *x, vector *y) {
  if (
    (x->size == y->size) &&
    (y->type_size == sizeof(double)) &&
    (x->type_size == sizeof(double))
     ) {
	  double result = 0.0;
	  for (size_t i = 0; i < x->size; ++i) {
	    result += ((vector_val(x, i) - vector_val(y, i)) *
                 (vector_val(x, i) - vector_val(y, i)));
	  }
	  return sqrt(result);
  }
  return 0.0;
}

double vector_sclr_prod(vector *v, vector *c) {
  double result = 0.0;
  for (size_t i = 0; i < v->size; ++i) {
    result += vector_val(v, i) * vector_val(c, i);
  }
  return result;
}

void vector_normalize(vector *v) {
  double norm = sqrt(vector_sclr_prod(v, v));
  for (size_t i = 0; i < v->size; ++i) {
    vector_val(v, i) /= norm;
  }
}

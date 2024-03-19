#include <stdio.h>

#include "../types/pair.h"
#include "../types/vector.h"

/* F@%(*$@* sh17 coded */

/* 
  I wanted to do linked list of pairs at first
  But I am toooooooo lazy
*/

int main(void) {
  
  size_t p, p_1;
  printf("size of polynoms?\n");
  scanf("%zu", &p);
  scanf("%zu", &p_1);

  vector poly, poly_1, result;
  vector_init(&poly, p, sizeof(pair));
  vector_init(&poly_1, p_1, sizeof(pair));
  vector_init(&result, p + p_1, sizeof(pair));

  printf("first poly?\n");
  for (size_t i = 0; i < p; ++i) {
    double tmp;
    scanf("%lf", &tmp);
    
    pair tmp_;
    tmp_.a = i;
    tmp_.b = tmp;
    vector_push(&poly, (void *)&tmp_);
  }

  printf("second poly?\n");
  for (size_t i = 0; i < p_1; ++i) {
    double tmp;
    scanf("%lf", &tmp);
    
    pair tmp_;
    tmp_.a = i;
    tmp_.b = tmp;
    vector_push(&poly_1, (void *)&tmp_);
  }

  double degree, coef;
  for (size_t i = 0; i < poly.size; ++i) {
    for (size_t j = 0; j < poly_1.size; ++j) {
      degree = unw_pair(vector_get(&poly, i)).a + 
                unw_pair(vector_get(&poly_1, j)).a;
      coef = unw_pair(vector_get(&poly, i)).b *
              unw_pair(vector_get(&poly_1, j)).b;
      
      pair tmp;
      tmp.a = degree;
      tmp.b = coef;
      
      vector_push(&result, (void *)&tmp);
    }
  }

  vector_print_pairs(&result);

  vector_free(&poly);
  vector_free(&poly_1);
  vector_free(&result);

  return 0;
}

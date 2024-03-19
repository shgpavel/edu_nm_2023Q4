#ifndef PAIR_H
#define PAIR_H

typedef struct pair_s {
  double a;
  double b;
} pair;

#define unw_double(ptr) (*(double *) (ptr))
#define unw_pair(ptr) (*(pair *) (ptr))

#endif

#ifndef PAIR_H
#define PAIR_H

typedef struct pair_s {
  double a;
  double b;
} pair;


#define unwrap_double(ptr) (*(double *) (ptr))
#define unwrap_pair(ptr) (*(pair *) (ptr))

#endif

#ifndef EIGENPAIR_H
#define EIGENPAIR_H

#include "vector.h"

typedef struct eigenpair_s {
  double eigenvalue;
  vector eigenvector;
} eigenpair;

#endif

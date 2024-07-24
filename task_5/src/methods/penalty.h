#ifndef PENALTY_H
#define PENALTY_H

#include "../types/vector.h"

vector *newton_poly_dd(vector *);
vector *spline_2_0(vector *, size_t, vector *);
vector *lagr_slae(vector *);

#endif

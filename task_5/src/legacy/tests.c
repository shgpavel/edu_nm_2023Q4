void test() {
  size_t sz;
  scanf("%zu", &sz);
  vector points;
  vector_init(&points, sz, sizeof(pair));

  for (size_t i = 0; i < sz; ++i) {
    pair pushed;
    scanf("%lf", &pushed.a);
    vector_push(&points, &pushed);
  }

  for (size_t i = 0; i < sz; ++i) {
    scanf("%lf", &(pair_get(&points, i)).b);
  }
  vector_print_pairs(&points);

  vector *res = lagr_slae(&points);
  vector_print(res);

  vector_free(res);
  free(res);
}

void test_spline() {
  size_t sz;
  scanf("%zu", &sz);
  vector points;
  vector_init(&points, sz, sizeof(pair));

  for (size_t i = 0; i < sz; ++i) {
    pair pushed;
    scanf("%lf", &pushed.a);
    vector_push(&points, &pushed);
  }

  for (size_t i = 0; i < sz; ++i) {
    scanf("%lf", &(pair_get(&points, i)).b);
  }
  vector_print_pairs(&points);

  vector *res = (vector *)malloc(sizeof(vector));
  vector_init(res, points.size - 1, sizeof(vector));

  /*
  for (size_t i = 0; i < points.size - 1; ++i) {
    res = linear_spline(&points, i, res);
  }

  for (size_t i = 0; i < points.size - 2; i += 2) {
    res = quad_spline(&points, i, res);
  }

  for (size_t i = 0; i < points.size - 3; i += 3) {
    res = qube_spline(&points, i, res);
  }
  */

  add_spline_func(res, &points);

  for (size_t i = 0; i < res->size; ++i) {
    vector_print(vector_get(res, i));
    vector_free(vector_get(res, i));
    free(vector_get(res, i));
  }

  plot(1);
  clear_plot();

  vector_free(res);
  free(res);
}

/*
void penalty_4(vector *points) {

  for (size_t i = 0; i < sz; ++i) {
    scanf("%lf", &(pair_get(&points, i)).b);
  }
  vector_print_pairs(&points);

  vector *res = (vector *)malloc(sizeof(vector));
  vector_init(res, points.size - 1, sizeof(vector));

  for (size_t i = 0; i < points.size - 2; i += 2) {
    res = spline_2_0(&points, i, res);
  }

  add_spline_func(res, &points);

  for (size_t i = 0; i < res->size; ++i) {
    vector_print(vector_get(res, i));
    vector_free(vector_get(res, i));
    free(vector_get(res, i));
  }

  plot(1);
  clear_plot();

  vector_free(res);
  free(res);
}
*/

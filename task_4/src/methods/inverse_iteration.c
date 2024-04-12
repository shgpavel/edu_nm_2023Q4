/*

matrix a;
    vector x_0, x;
    double tmp, epsilon;
    size_t n;

    printf("matrix size?\n");
    scanf("%zu", &n);

    matrix_init(&a, n, sizeof(double));
    vector_init(&x_0, n, sizeof(double));
    vector_init(&x, n, sizeof(double));
    
    for (size_t i = 0; i < n; ++i) {
        tmp = 1.0;
        vector_push(&x, &tmp);
    }

    printf("matrix?\n");
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            scanf("%lf", &tmp);
            matrix_push(&a, (void *)&tmp);
        }
    }

    printf("eigen values?\n");
    for (size_t i = 0; i < n; ++i) {
        scanf("%lf", &tmp);
        vector_push(&x_0, &tmp);
    }

    printf("epsilon?\n");
    scanf("%lf", &epsilon);

    matrix_dm(&a, &x_0);
   
    do {
        vector_normalize(&a, &x_0);
        lup(&a, &x_0, &x);
        //vector_normalize(&a, &x_0);
        vector_assign(&x_0, &x);
    } while (vector_diff(&x, &x_0) > epsilon); 

    vector_print(&x);

*/

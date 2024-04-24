/*
    matrix_dm(&a, &x_0);
   
    do {
        vector_normalize(&a, &x_0);
        lup(&a, &x_0, &x);
        //vector_normalize(&a, &x_0);
        vector_assign(&x_0, &x);
    } while (vector_diff(&x, &x_0) > epsilon); 

    vector_print(&x);

*/

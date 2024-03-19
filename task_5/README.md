Contains:
=========
    src/types/vector.c          -- vector with jemalloc (if ! use libc instead)
    src/types/matrix.c          -- matrix (only 1 vector is used)
    src/methods/lup.c           -- LUP matrix solver
    src/func/func.c             -- function
    src/test/1                  -- example test data

Dependencies:
=============
    jemalloc >= 5.3
    clang    >= 15

Build:
======
    make
    make clean

Status:
=======
    Working and closed
    It means I leave it in current stage and
    improvements from further tasks will not be
    commited here

Contains:
=========
    src/vector/vector.c          -- vector with jemalloc (if ! use libc instead)
    src/matrix/matrix.c          -- matrix (only 1 vector is used)
    src/lup/lup.c                -- LUP-decompose for this vector and matrix
    src/func/func.c              -- functions for newton method
    src/test/1                   -- example test data

Dependencies:
=============
    jemalloc >= 5.3
    clang    >= 15

Build:
======
    make
    make clean

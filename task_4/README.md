Overview:
=========
    It was my enthusiastic task but now
    I received suitable objective for this
    method described in tz_4.pdf

    Also I need to mention that this task I'm
    doing after the task_5 so it probably would
    not be done before task_5

Status:
=======
    Not debugged not working yet huh

Contains:
=========
    ../task_3/src/vector/vector.c          -- vector Makefile rule (from task_3 to lib)
    ../task_3/src/matrix/matrix.c          -- matrix Makefile rule (from task_3 to lib)
    ../task_3/src/lup/lup.c                -- LUP-decompose Makefile rule (from task_3 to lib)
    src/main.c                             -- inverse iteration method
    src/test/1                             -- example test data

Dependencies:
=============
    jemalloc >= 5.3
    clang    >= 15

Build:
======
    make liblina.so     // makes lib from task_3 matrix and vector
    make install        // install lib to /usr/lib
    make uninstall      // uninstall lib
    make main           // make main.c (the method)
    make clean

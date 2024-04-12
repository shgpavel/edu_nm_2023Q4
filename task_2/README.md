Status:
=======
    Working and closed
    It means I leave it in current stage and
    improvements from further tasks will not be
    commited here

Contains:
===========
    src/main.cpp                 -- Class Vector, Matrix impl + tests
    src/MPI/mpi.cpp              -- Impl of simple iteration method
    src/MS/ms.cpp                -- Impl of Seidel method
    src/LUP/lup.cpp              -- LUP-decompose method
    src/QR/qr.cpp                -- QRH-decompose method
    src/EigenWrap/eigen_wrap.cpp -- Eigen wrap for classes Vector, Matrix
    src/Funcs/funcs.cpp          -- Heron's mehod for square root
    src/test/1                   -- First 5 tests (0-4)

Dependencies:
============
    eigen >= 3.4
    clang >= 15

Build:
=======
    make
    make clean

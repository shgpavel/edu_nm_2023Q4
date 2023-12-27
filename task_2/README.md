Содержимое:
===========
    src/main.cpp                 -- реализация классов Vector, Matrix + тесты методов  
    src/MPI/mpi.cpp              -- реализация метода простой итерации
    src/MS/ms.cpp                -- реализация метода Зейделя
    src/LUP/lup.cpp              -- реализация метода LUP-разложения
    src/QR/qr.cpp                -- реализация метода QR-разложения
    src/EigenWrap/eigen_wrap.cpp -- обёртка библиотеки eigen для классов Vector, Matrix
    src/Funcs/funcs.cpp          -- вычисление корня методом Герона
    src/test/1                   -- первые 5 тестов (0-4)

Зависимости:
============
    eigen >= 3.4
    clang >= 15

Сборка:
=======
    make                         -- build main.o executable file
    make clean                   -- rm main.o

Содержимое:
===========
    src/main.c                   -- задание  
    src/vector/vector.c          -- вектор с кастомным аллокатором (if ! use libc instead)  
    src/matrix/matrix.c          -- матрица (уже нормальная как один вектор)  
    src/lup/lup.c                -- реализация метода LUP разложения  
    src/test/1                   -- пример данных для теста

Зависимости:
============
    jemalloc >= 5.3  
    clang    >= 15

Сборка:
=======
    make                         -- build main.o executable file
    make clean                   -- rm main.o

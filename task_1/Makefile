CFLAGS := ${CFLAGS} -Wall -Wextra -O3 -std=c++20

CC = clang++
LD = clang++

main: main.cpp
	$(CC) main.cpp funcs.cpp -o main.o $(CFLAGS)

clean:
	rm -rf main.o

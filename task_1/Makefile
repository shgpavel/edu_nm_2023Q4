CFLAGS := ${CFLAGS} -Wall -Wextra -O3 -std=c++2b

CC ?= clang++
LD ?= clang++

main: main.cpp
	$(CC) main.cpp funcs.cpp -o main.o $(CFLAGS)

ref: ref.cpp
	$(CC) ref.cpp -o ref.o $(CFLAGS)

clean:
	rm -rf main.o ref.o

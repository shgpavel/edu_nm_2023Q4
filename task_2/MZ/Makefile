CFLAGS := ${CFLAGS} -Wall -Wextra -O3 -std=c++2b

CC = clang++
LD = clang++

main: main_MPI.cpp
	$(CC) main_MPI.cpp -o main.o $(CFLAGS)

clean:
	rm -rf main.o

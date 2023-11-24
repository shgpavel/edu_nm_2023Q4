CFLAGS := ${CFLAGS} -Wall -Wextra -O3 -std=c++2b

CC = clang++
LD = clang++

main: main_LUP.cpp
	$(CC) main_LUP.cpp -o main.o $(CFLAGS)

clean:
	rm -rf main.o

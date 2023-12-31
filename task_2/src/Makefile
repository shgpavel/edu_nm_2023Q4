CFLAGS := -Wall -Wextra -O3 -std=c++2b ${CFLAGS}

CC = clang++
LD = clang++

SRCS_DIRS = . MPI MS LUP QR EigenWrap Funcs
SRCS = $(wildcard $(addsuffix /*.cpp, $(SRCS_DIRS)))

main: main.cpp
	$(CC) $(SRCS) -o main.o $(CFLAGS)

clean:
	rm -rf main.o

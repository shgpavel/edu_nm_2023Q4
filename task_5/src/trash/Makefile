CFLAGS := -Wall -Wextra -O3 -std=c2x ${CFLAGS}

CC = clang
LD = clang

SRCS = test_poly.c ../types/vector.c

LL = -ljemalloc -lm

main: $(SRCS)
	$(CC) $(SRCS) -o main.o $(LL) $(CFLAGS)

clean:
	rm -rf main.o

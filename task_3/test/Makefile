CFLAGS := -Wall -Wextra -O3 -std=c2x ${CFLAGS}

CC = clang
LD = clang

SRCS_DIRS = ../lup ../matrix ../vector
SRCS = $(wildcard $(addsuffix /*.c, $(SRCS_DIRS)))

LL = -lm -ljemalloc

test_1: $(SRCS) test_lup.c
	$(CC) $(SRCS) test_lup.c -o test.o $(LL) $(CFLAGS)


clean:
	rm -rf test.o

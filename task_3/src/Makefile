CFLAGS := -Wall -Wextra -O3 -std=c2x ${CFLAGS}

CC = clang
LD = clang

SRCS_DIRS = . lup matrix vector func
SRCS = $(wildcard $(addsuffix /*.c, $(SRCS_DIRS)))

LL = -lm -ljemalloc

main: $(SRCS)
	$(CC) $(SRCS) -o main.o $(LL) $(CFLAGS)

clean:
	rm -rf main.o

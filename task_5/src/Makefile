CFLAGS := -Wall -Wextra -O0 -std=gnu2x ${CFLAGS}

CC = clang
LD = clang

SRCS_DIRS = . methods types funcs draw
SRCS = $(wildcard $(addsuffix /*.c, $(SRCS_DIRS)))

LL = -lm -ljemalloc

main: $(SRCS)
	$(CC) $(SRCS) -o main.o $(LL) $(CFLAGS)

clean:
	rm -rf main.o

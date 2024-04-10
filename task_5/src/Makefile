CFLAGS := -Wall -Wextra -O3 -std=gnu2x ${CFLAGS}

CC = clang
LD = clang

SRCS_DIRS = . methods types func draw
SRCS = $(wildcard $(addsuffix /*.c, $(SRCS_DIRS)))

LL = -lm -ljemalloc -lcurl

main: $(SRCS)
	$(CC) $(SRCS) -o main.o $(LL) $(CFLAGS)

clean:
	rm -rf main.o

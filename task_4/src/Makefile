CFLAGS := -Wall -Wextra -O3 -std=gnu2x ${CFLAGS}

CC = gcc
LD = gcc

SRCS_DIRS = . methods types
SRCS = $(wildcard $(addsuffix /*.c, $(SRCS_DIRS)))

LL = -lm -ljemalloc -lpthread

main: $(SRCS)
	$(CC) $(SRCS) -o $(@) $(LL) $(CFLAGS)

clean:
	rm -rf main

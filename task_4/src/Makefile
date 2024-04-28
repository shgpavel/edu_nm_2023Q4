CFLAGS := -Wall -Wextra -O1 -std=gnu18 ${CFLAGS}

CC = gcc
LD = gcc

SRCS_DIRS = . methods types
SRCS = $(wildcard $(addsuffix /*.c, $(SRCS_DIRS)))

LL = -lm -ljemalloc

main: $(SRCS)
	$(CC) $(SRCS) -pthread -o $(@) $(LL) $(CFLAGS)

clean:
	rm -rf main

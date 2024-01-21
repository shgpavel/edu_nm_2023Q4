CFLAGS := -Wall -Wextra -O3 -std=c2x ${CFLAGS}

CC = clang
LD = clang

SRCS_LIB = ../../task_3/src/lup/lup.c \
			../../task_3/src/matrix/matrix.c \
			../../task_3/src/vector/vector.c

SRCS_DIRS = .
SRCS = $(wildcard $(addsuffix /*.c, $(SRCS_DIRS)))

LL = -lm -ljemalloc

liblina.so: $(SRCS_LIB)
	$(CC) $(SRCS_LIB) -fPIC -shared -o $@ $(LL) $(CFLAGS)

main: $(SRCS)
	$(CC) $(SRCS) -o $@ -llina $(LL) $(CFLAGS)

install:
	sudo cp liblina.so /usr/lib

uninstall:
	sudo rm -rf /usr/lib/liblina.so

clean:
	rm -rf liblina.so main

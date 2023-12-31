CFLAGS := -Wall -Wextra -O3 -std=c2x ${CFLAGS}

CC = clang
LD = clang

main: main.c
	$(CC) main.c funcs.c -o main.o -lm $(CFLAGS)

clean:
	rm -rf main.o out.csv

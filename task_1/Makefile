CFLAGS := ${CFLAGS} -Wall -Wextra -O3 -std=c2x

CC = clang
LD = clang

main: main.c
	$(CC) main.c funcs.c -o main.o -lm $(CFLAGS)

clean:
	rm -rf main.o output.csv

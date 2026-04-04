CC = gcc
CFLAGS = -Wall -Wextra -std=c11

.PHONY: all run test clean

all: astar

astar: astar.c
	$(CC) $(CFLAGS) astar.c -o astar

run: astar
	./astar

test: astar_tests
	./astar_tests

astar_tests: astar_tests.c
	$(CC) $(CFLAGS) astar_tests.c -o astar_tests

clean:
	rm -f astar astar_tests main

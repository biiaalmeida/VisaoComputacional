CC = gcc
CFLAGS = -Wall -Wextra -std=c11

.PHONY: all run test clean

all: astar astar_teste

astar: astar.c mapa_preprocessamento.c
	$(CC) $(CFLAGS) astar.c mapa_preprocessamento.c -o astar

run: astar
	./astar

test: astar_teste
	./astar_teste

astar_teste: astar_teste.c mapa_preprocessamento.c
	$(CC) $(CFLAGS) astar_teste.c mapa_preprocessamento.c -o astar_teste

clean:
	rm -f astar astar_teste main

CC = gcc
CFLAGS = -Wall -Wextra -std=c11

SRC_DIR = c
ASTAR_SRC = $(SRC_DIR)/astar.c
TEST_SRC = $(SRC_DIR)/astar_teste.c
MAPA_SRC = $(SRC_DIR)/mapa_preprocessamento.c

.PHONY: all run test clean activate venv-check run-image

all: astar astar_teste

astar: $(ASTAR_SRC) $(MAPA_SRC)
	$(CC) $(CFLAGS) $(ASTAR_SRC) $(MAPA_SRC) -o $@

run: astar
	./astar

test: astar_teste
	./astar_teste

astar_teste: $(TEST_SRC) $(ASTAR_SRC) $(MAPA_SRC)
	$(CC) $(CFLAGS) $(TEST_SRC) $(MAPA_SRC) -o $@

activate:
	@echo "source .venv/bin/activate"

venv-check:
	@.venv/bin/python -c "import numpy, cv2; print('OK', numpy.__version__, cv2.__version__)"

run-image:
	@.venv/bin/python python/src/gerarimagem.py

clean:
	rm -f astar astar_teste main

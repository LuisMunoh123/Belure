CC=gcc
EXEC=player.out
GRUPO=G1
NTAR=2

SRC_DIR=src
OBJ_DIR=obj
SRC_FILES=$(wildcard $(SRC_DIR)/*.c)
OBJ_FILES=$(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))
INCLUDE=-I./incs/
LIBS= -lm

CFLAGS=-Wall -Wextra -Wpedantic -O3
LDFLAGS= -Wall -lm

# pero no creo que haya caido tan bajo como un !GUION BAJO!. 
# player.h

PYTHON=python3
ALLEGRO6_DIR=allegro6
ALLEGRO6_VENV=$(ALLEGRO6_DIR)/.venv
ALLEGRO6_PY=$(ALLEGRO6_DIR)/main.py
ALLEGRO6_REQ=$(ALLEGRO6_DIR)/requirements.txt
ALLEGRO6_INSTALLER=$(ALLEGRO6_DIR)/install-allegro6.sh

all: folders $(OBJ_FILES)
	$(CC) $(CFLAGS) -o build/$(EXEC) $(OBJ_FILES) $(INCLUDE) $(LIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $^ $(INCLUDE)

.PHONY: clean folders run send install-allegro6 plot

clean:
	rm -f $(OBJ_FILES)
	rm -rf build/*.*
	rm -rf build/db/*
	rm -f *.gif
	rm -f *.png

folders:
	mkdir -p src obj incs build/db docs/results $(ALLEGRO6_DIR)

run:
	@./build/$(EXEC)

install-allegro6:
	@bash $(ALLEGRO6_INSTALLER)

plot:
	@if [ -x $(ALLEGRO6_VENV)/bin/python ]; then \
		$(ALLEGRO6_VENV)/bin/python $(ALLEGRO6_PY); \
	else \
		$(PYTHON) $(ALLEGRO6_PY); \
	fi

send:
	tar czf $(GRUPO)-$(NTAR).tgz --transform 's,^,$(GRUPO)-$(NTAR)/,' Makefile src incs docs $(ALLEGRO6_DIR)


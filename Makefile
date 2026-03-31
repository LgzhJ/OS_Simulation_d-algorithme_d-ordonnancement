# Makefile — Simulateur d'ordonnancement OS
# Usage :
#   make          -> compile le projet
#   make clean    -> supprime les fichiers compilés
#   make doc      -> génère la doc Doxygen
#   make install  -> installe le binaire

CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11 -Iinclude
SRC_DIR = src
OBJ_DIR = obj
BIN     = scheduler

SRCS = $(SRC_DIR)/main.c \
	   $(SRC_DIR)/file.c \
       $(SRC_DIR)/fifo.c \
       $(SRC_DIR)/sjf.c \
	   $(SRC_DIR)/rr.c \
       $(SRC_DIR)/metrics.c

OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# ── Cible par défaut ────────────────────────────────────────────────────
all: $(OBJ_DIR) $(BIN)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# ── Nettoyage ───────────────────────────────────────────────────────────
clean:
	rm -rf $(OBJ_DIR) $(BIN) resultats_*.csv

# ── Documentation Doxygen ───────────────────────────────────────────────
doc:
	doxygen Doxyfile

# ── Installation ────────────────────────────────────────────────────────
install: $(BIN)
	@if [ -w /usr/local/bin ]; then \
		cp $(BIN) /usr/local/bin/$(BIN); \
		echo "Installé dans /usr/local/bin/$(BIN)"; \
	else \
		mkdir -p $(HOME)/bin; \
		cp $(BIN) $(HOME)/bin/$(BIN); \
		echo "Installé dans $(HOME)/bin/$(BIN)"; \
	fi

.PHONY: all clean doc install

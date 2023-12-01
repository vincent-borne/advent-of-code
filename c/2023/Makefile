SRC_DIR = src/$(day)
BUILD_DIR = build/debug
CC = gcc
SRC_FILES = $(wildcard $(SRC_DIR)/part_$(part).c)
OBJ_NAME = aoc
INCLUDE_PATHS =
LIBRARY_PATHS =
COMPILER_FLAGS = -Wall -Wextra -O0 -g
LINKER_FLAGS =

all:
	$(CC) $(COMPILER_FLAGS) $(LINKER_FLAGS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(SRC_FILES) -o $(BUILD_DIR)/$(OBJ_NAME)
# ================================
# Root Makefile - Recursive Build
# ================================

# Macros
export CC = gcc
export CFLAGS = -Wall -Wextra -g
export INCLUDE_DIR = $(CURDIR)/include
export BIN_DIR = $(CURDIR)/bin
export TARGET = client

SRC_DIR = src

.PHONY: all clean run rebuild

all:
	@echo "Building project..."
	@mkdir -p $(BIN_DIR)
	$(MAKE) -C $(SRC_DIR)
	@echo "Build complete: $(BIN_DIR)/$(TARGET)"

run: all
	@echo "Running $(TARGET)..."
	$(BIN_DIR)/$(TARGET)

clean:
	@echo "Cleaning project..."
	$(MAKE) -C $(SRC_DIR) clean
	rm -rf $(BIN_DIR)
	@echo "Clean complete."

rebuild: clean all

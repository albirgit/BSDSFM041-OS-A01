# ================================
# Root Makefile - Recursive Build
# ================================

export CC = gcc
export CFLAGS = -Wall -Wextra -g
export INCLUDE_DIR = $(CURDIR)/include
export BIN_DIR = $(CURDIR)/bin
export LIB_DIR = $(CURDIR)/lib
export TARGET = client_static
export LIBNAME = myutils

SRC_DIR = src

.PHONY: all clean run rebuild install uninstall

all:
	@echo "Building project..."
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(LIB_DIR)
	$(MAKE) -C $(SRC_DIR)
	@echo "Build complete: $(BIN_DIR)/$(TARGET)"

run: all
	@echo "Running $(TARGET)..."
	$(BIN_DIR)/$(TARGET)

install: all
	@echo "Installing $(TARGET)..."
	@cp $(BIN_DIR)/$(TARGET) /usr/local/bin/
	@chmod 755 /usr/local/bin/$(TARGET)
	@echo "Installation complete."

uninstall:
	@rm -f /usr/local/bin/$(TARGET)
	@echo "Uninstalled $(TARGET)."

clean:
	@echo "Cleaning project..."
	$(MAKE) -C $(SRC_DIR) clean
	rm -rf $(BIN_DIR)
	rm -rf $(LIB_DIR)
	@echo "Clean complete."

rebuild: clean all

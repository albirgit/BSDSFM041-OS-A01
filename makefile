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

.PHONY: all clean run rebuild install uninstall

install: all
	@echo "Installing client..."
	@cp $(BIN_DIR)/client_static /usr/local/bin/client
	@chmod 755 /usr/local/bin/client
	@echo "Installing man pages..."
	@mkdir -p /usr/local/share/man/man3
	@cp man/man3/*.3 /usr/local/share/man/man3/
	@mandb > /dev/null 2>&1 || true
	@echo "Installation complete."

uninstall:
	@echo "Uninstalling client..."
	@rm -f /usr/local/bin/client
	@rm -f /usr/local/share/man/man3/mystrlen.3
	@rm -f /usr/local/share/man/man3/mystrcpy.3
	@rm -f /usr/local/share/man/man3/mystrncpy.3
	@rm -f /usr/local/share/man/man3/mystrcat.3
	@rm -f /usr/local/share/man/man3/wordCount.3
	@rm -f /usr/local/share/man/man3/mygrep.3
	@mandb > /dev/null 2>&1 || true
	@echo "Uninstalled."
clean:
	@echo "Cleaning project..."
	$(MAKE) -C $(SRC_DIR) clean
	rm -rf $(BIN_DIR)
	rm -rf $(LIB_DIR)
	@echo "Clean complete."

rebuild: clean all

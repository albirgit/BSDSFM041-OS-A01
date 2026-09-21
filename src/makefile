# ================================
# src/Makefile - Compiles sources
# ================================

# Macros (CC, CFLAGS, INCLUDE_DIR, BIN_DIR, TARGET inherited from root Makefile)
CFLAGS += -I$(INCLUDE_DIR)

SOURCES = main.c mystrfunctions.c myfilefunctions.c
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = $(BIN_DIR)/$(TARGET)

.PHONY: all clean

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS)
	rm -f $(EXECUTABLE)

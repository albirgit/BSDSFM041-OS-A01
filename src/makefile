# ================================
# src/Makefile - Compiles sources
# ================================

CFLAGS += -I$(INCLUDE_DIR)

SOURCES = main.c mystrfunctions.c myfilefunctions.c
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = $(BIN_DIR)/$(TARGET)

.PHONY: all clean

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS)
	rm -f $(EXECUTABLE)

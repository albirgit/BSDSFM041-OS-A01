# ================================
# src/Makefile - Compiles sources
# ================================

CFLAGS += -I$(INCLUDE_DIR)

# Object files that go into the static library
LIB_OBJECTS = mystrfunctions.o myfilefunctions.o

# Main program object
MAIN_OBJECT = main.o

STATIC_LIB = $(LIB_DIR)/lib$(LIBNAME).a
EXECUTABLE = $(BIN_DIR)/$(TARGET)

.PHONY: all clean

all: $(EXECUTABLE)

# Step 1: Build the static library from mystrfunctions.o and myfilefunctions.o
$(STATIC_LIB): $(LIB_OBJECTS)
	ar rcs $@ $^

# Step 2: Link main.o against the static library
$(EXECUTABLE): $(MAIN_OBJECT) $(STATIC_LIB)
	$(CC) $(CFLAGS) -o $@ $(MAIN_OBJECT) -L$(LIB_DIR) -l$(LIBNAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o
	rm -f $(EXECUTABLE)
	rm -f $(STATIC_LIB)

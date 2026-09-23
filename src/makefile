# ================================
# src/Makefile - Compiles sources
# ================================

CFLAGS += -I$(INCLUDE_DIR)

LIB_OBJECTS = mystrfunctions.o myfilefunctions.o
PIC_OBJECTS = mystrfunctions_pic.o myfilefunctions_pic.o
MAIN_OBJECT = main.o

STATIC_LIB = $(LIB_DIR)/lib$(LIBNAME).a
DYNAMIC_LIB = $(LIB_DIR)/lib$(LIBNAME).so

STATIC_EXECUTABLE = $(BIN_DIR)/client_static
DYNAMIC_EXECUTABLE = $(BIN_DIR)/client_dynamic

.PHONY: all clean

all: $(STATIC_EXECUTABLE) $(DYNAMIC_EXECUTABLE)

# -------------------------------
# Static Library
# -------------------------------

$(STATIC_LIB): $(LIB_OBJECTS)
	ar rcs $@ $^

$(STATIC_EXECUTABLE): $(MAIN_OBJECT) $(STATIC_LIB)
	$(CC) $(CFLAGS) -o $@ $(MAIN_OBJECT) $(STATIC_LIB)

# -------------------------------
# Dynamic Library
# -------------------------------

$(DYNAMIC_LIB): $(PIC_OBJECTS)
	$(CC) -shared -o $@ $^

$(DYNAMIC_EXECUTABLE): $(MAIN_OBJECT) $(DYNAMIC_LIB)
	$(CC) $(CFLAGS) -o $@ $(MAIN_OBJECT) -L$(LIB_DIR) -l$(LIBNAME)

# -------------------------------
# Normal Object Files
# -------------------------------

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# -------------------------------
# Position Independent Object Files
# -------------------------------

%_pic.o: %.c
	$(CC) $(CFLAGS) -fPIC -c $< -o $@

# -------------------------------
# Clean
# -------------------------------

clean:
	rm -f *.o
	rm -f $(STATIC_LIB)
	rm -f $(DYNAMIC_LIB)
	rm -f $(STATIC_EXECUTABLE)
	rm -f $(DYNAMIC_EXECUTABLE)

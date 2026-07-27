# Standalone Makefile-based module (not built by CMake directly).
# Produces a static library that the top-level project links against.

CC      ?= cc
AR      ?= ar
CFLAGS  ?= -Wall -O2

LIB := libhello_world.a
OBJ := hello_world.o

.PHONY: all clean

all: $(LIB)

$(LIB): $(OBJ)
	$(AR) rcs $@ $^

hello_world.o: hello_world.c hello_world.h
	$(CC) $(CFLAGS) -c hello_world.c -o hello_world.o

clean:
	rm -f $(OBJ) $(LIB)

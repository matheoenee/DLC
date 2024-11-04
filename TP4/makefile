# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -g -O2

# GMP library
LIBS = -lgmp

# Target executable name
TARGET = ecdsa_keygen

# Source files
SRCS = ecdsa_keygen.c add_and_double.c

# Default target
all: $(TARGET)

# Build the executable directly from source files
$(TARGET): $(SRCS) add_and_double.h struct.h
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS) $(LIBS)

# Clean up generated files
clean:
	rm -f $(TARGET)

# Rebuild from scratch
rebuild: clean all

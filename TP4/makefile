# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -g -O2

# GMP and OpenSSL libraries
LIBS = -lgmp -lcrypto

# Target executable name
TARGET = ecdsa_verif

# Source files
SRCS = ecdsa_verif.c add_and_double.c read_data.c sha256.c

# Default target
all: $(TARGET)

# Build the executable directly from source files
$(TARGET): $(SRCS) add_and_double.h struct.h read_data.h sha256.h
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS) $(LIBS)

# Clean up generated files
clean:
	rm -f $(TARGET)

# Rebuild from scratch
rebuild: clean all

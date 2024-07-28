# Choose the compiler: gcc or clang
CC = gcc
#CC = clang

# Compiler and linker flags
CFLAGS = -Wall -O3
LDFLAGS = -lzint -lhpdf -lpng -lz

# Source files
SRCS = labels.c

# Output binary
TARGET = labels

# Default target
all: $(TARGET)

# Compile the program
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS) $(LDFLAGS)

# Clean up generated files
clean:
	rm -f $(TARGET)
	rm -f ./output/*.pdf

# Run the program with sample arguments
run: $(TARGET)
	./$(TARGET) 3 28 30 6

.PHONY: all clean run

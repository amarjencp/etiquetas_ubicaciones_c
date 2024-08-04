CC = gcc
CFLAGS = -Wall -O3
LDFLAGS = -lzint -lhpdf -lpng -lz

SRCS = labels.c
TARGET = labels

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS) $(LDFLAGS)

clean:
	rm -f $(TARGET)
	rm -f ./output/*.pdf

run: $(TARGET)
	./$(TARGET) 3 28 30 6

.PHONY: all clean run

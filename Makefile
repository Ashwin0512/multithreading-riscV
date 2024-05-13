CC = gcc
CFLAGS = -Wall -Wextra -pthread
TARGET = main
SCRIPT = script.sh

.PHONY: all clean

all: $(TARGET)

$(TARGET): main.c
	$(CC) $(CFLAGS) -o $@ $^

run: $(TARGET) $(SCRIPT)
	./$(SCRIPT)

clean:
	rm -f $(TARGET)
CC = cc
CFLAGS = -Wall -Wextra -g
LIBS = -lm
TARGET = main
SRC = main.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LIBS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all run clean
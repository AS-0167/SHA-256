CC = gcc
TARGET = sha-256-hash
PREFIX = /usr/local/bin

SRC = sha-256.c
OBJ = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $(TARGET) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

install: $(TARGET)
	install -m 755 $(TARGET) $(PREFIX)/$(TARGET)

uninstall:
	rm -f $(PREFIX)/$(TARGET)

run: $(TARGET)
	./$(TARGET) --fpath input.txt --sha256path output.txt

delete:
	rm -f $(TARGET)

clean:
	rm -f $(OBJ) $(TARGET)

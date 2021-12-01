PROGRAM = crc32
.PHONY: all clean install

CC = CLANG
CFLAGS = -static-libgcc -fcolor-diagnostics --target=x86_64-w64-mingw -std=c11
SRC = main.c

all: $(PROGRAM)

$(PROGRAM): $(SRC)
	$(CC) -o $@ -Wall -Winline -std=c99 $(CFLAGS) $(LDFLAGS) $(SRC)

clean:
	-rm -f $(PROGRAM)

install:
	install -m 0755 -d $(PREFIX)/bin
	install -m 0755 $(PROGRAM) $(PREFIX)/bin

CC=cc
CFLAGS=-Wall -Wextra -Wshadow -pedantic -O2 -ansi
EXEC=ipaddrconv
SRC=ipaddrconv.c

all: $(EXEC)

$(EXEC): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^

srcdir=src
objdir=obj
bindir=bin

CC=gcc
CFLAGS=-Wall -ansi -pedantic
LDFLAGS=

BIN=fleury
SRC= $(wildcard $(srcdir)/*.c)
OBJ= $(SRC:$(srcdir)/%.c=$(objdir)/%.o)

all: depend prepare $(BIN)

prepare:
	mkdir -p $(objdir) $(bindir)

$(BIN): $(OBJ)
	$(CC) -o $(bindir)/$@ $^ $(LDFLAGS) $(CFLAGS)

depend:
	$(CC) -MM $(SRC) >depend

-include depend

$(objdir)/%.o: $(srcdir)/%.c
	$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: install clean cleaninstall mrproper

clean:
	rm -rf $(objdir)

mrproper: clean
	rm -rf $(srcdir)/*~
	rm -rf $(bindir)
	rm -f depend
CC = gcc

CFLAGS = -I.
DEBUGFLAGS = -g -O0

DEPS = src/linux/tsekL.h src/tsekI.h
OBJ = main.o src/tsekI.o src/linux/tsekL.o

LDFLAGS = -lX11 -lc -lGL

# normal build
tsekI: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

# debug build (for valgrind)
debug: CFLAGS += $(DEBUGFLAGS)
debug: clean tsekI

# object compilation
%.o: %.c $(DEPS)
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -f $(OBJ) tsekI

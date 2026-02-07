CC = gcc

CFLAGS = -I.
DEBUGFLAGS = -g -O0

DEPS = src/linux/tsekL.h src/tsekI.h libs/glad.h libs/khrplatform.h src/tsekG.h
OBJ = main.o src/tsekI.o src/linux/tsekL.o libs/glad.o src/tsekG.o

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

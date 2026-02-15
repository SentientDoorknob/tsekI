CC = gcc

CFLAGS = -I.
DEBUG_FLAGS = -g -O0

LDFLAGS_LINUX = -lX11 -lc -lGL
LDFLAGS_WINDOWS = 

LDFLAGS = 

DEPS = src/tsekI.h src/tsekG.h libs/glad.h src/linux/tsekL.h src/windows/tsekW.h
OBJS = main.o src/tsekI.o src/tsekG.o libs/glad.o src/linux/tsekL.o src/windows/tsekW.o

CFLAGS_WINDOWS = -DPLATFORM_WINDOWS -UPLATFORM_LINUX
CFLAGS_LINUX = -DPLATFORM_LINUX -UPLATFORM_WINDOWS

ifeq ($(p), windows)
	CFLAGS += $(CFLAGS_WINDOWS)
	LDFLAGS = $(LDFLAGS_WINDOWS)
endif

ifeq ($(p), linux)
	CFLAGS += $(CFLAGS_LINUX)
	LDFLAGS = $(LDFLAGS_LINUX)
endif

ifeq ($(d), 1)
	CFLAGS += $(DEBUG_FLAGS)
endif


.PHONY: all tsekI clean

all: tsekI

tsekI: $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c $(DEPS)
	$(CC) -c $< -o $@ $(CFLAGS)

clean: 
	rm -f $(OBJS) tsekI

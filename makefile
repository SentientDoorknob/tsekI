CC = gcc

CFLAGS = -I.
DEBUG_FLAGS = -g -O0

LDFLAGS_LINUX = -lX11 -lc -lGL
LDFLAGS_WINDOWS = 

LDFLAGS = 

DEPS = src/tsekI.h src/tsekG.h src/linux/tsekL.h src/windows/tsekW.h libs/glad.h
OBJS = main.o src/tsekI.o src/tsekG.o src/linux/tsekL.o src/windows/tsekW.o libs/glad.o

DEPS_LINUX = 
DEPS_WINDOWS = 

LIBS_LINUX = 
LIBS_WINDOWS =

CFLAGS_WINDOWS = -DPLATFORM_WINDOWS -UPLATFORM_LINUX
CFLAGS_LINUX = -DPLATFORM_LINUX -UPLATFORM_WINDOWS

ifeq ($(p), windows)
	CC = x86_64-w64-mingw32-gcc
	CFLAGS += $(CFLAGS_WINDOWS)
	LDFLAGS = $(LDFLAGS_WINDOWS)
	DEPS += $(DEPS_WINDOWS)
	OBJS += $(LIBS_WINDOWS)
endif

ifeq ($(p), linux)
	CFLAGS += $(CFLAGS_LINUX)
	LDFLAGS = $(LDFLAGS_LINUX)
	DEPS += $(DEPS_LINUX)
	OBJS += $(LIBS_LINUX)
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

CC = gcc

CFLAGS = -I.
DEBUG_FLAGS = -g -O0 -save-temps

LDFLAGS_LINUX = -lX11 -lc -lGL -lm -Lopenssl/openssl-0.9.8k/ -lssl -lcrypto -Iopenssl/openssl-0.9.8k/include
LDFLAGS_WINDOWS = -lopengl32 -lgdi32 -lwinmm -lws2_32

LDFLAGS = 

DEPS = src/tsekI.h src/tsekG.h src/linux/tsekL.h src/windows/tsekW.h libs/glad.h src/tsekM.h
OBJS = main.o src/tsekI.o src/tsekG.o src/linux/tsekL.o src/windows/tsekW.o libs/glad.o src/tsekM.o

DEPS_LINUX = 
DEPS_WINDOWS = 

LIBS_LINUX = 
LIBS_WINDOWS =

CFLAGS_WINDOWS = -DPLATFORM_WINDOWS -UPLATFORM_LINUX
CFLAGS_LINUX = -DPLATFORM_LINUX -UPLATFORM_WINDOWS

# auto-detect OS if 'p' is not provided
ifeq ($(p),)
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        p = linux
    endif
    ifeq ($(UNAME_S),Darwin)
        p = mac
    endif
    ifeq ($(OS),Windows_NT)
        p = windows
    endif
endif

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

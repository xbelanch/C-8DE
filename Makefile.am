bin_PROGRAMS = chip8-emu
chip8_emu_SOURCES = src/chip8.c src/yace.c src/yace.h\
src/opcodes.c src/debug.c src/video.c src/keyboard.c src/timer.c

AM_CFLAGS = -Wall
AM_CPPFLAGS = $(SDL_CFLAGS)
LDADD = $(SDL_LIBS) $(SDL_TTF_LIBS)

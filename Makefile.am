bin_PROGRAMS = yace
chip8_SOURCES = yace.c chip8.c yace.c yace.h memory.c opcodes.c debug.c video.c keyboard.c timer.c

AM_CFLAGS = -Wall
AM_CPPFLAGS = $(SDL_CFLAGS)
LDADD = $(SDL_LIBS) $(SDL_TTF_LIBS)

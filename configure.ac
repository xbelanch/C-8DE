#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.65])
AC_INIT([yace], [0.0], [xbelanch@gmail.com])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([foreign -Wall -Werror])
AC_CONFIG_SRCDIR([chip8.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.
PKG_CHECK_MODULES([SDL], [sdl >= 1.2])
AC_CHECK_LIB([SDL_ttf], [TTF_Init], [SDL_TTF_LIBS=-lSDL_ttf],
             [AC_MSG_ERROR([SDL_ttf library not found])])
AC_SUBST([SDL_TTF_LIBS])

# Checks for header files.
                 
# Checks for typedefs, structures, and compiler characteristics.
AC_PROG_CC_C99

# Checks for library functions.

AC_CONFIG_FILES([Makefile])
AC_OUTPUT

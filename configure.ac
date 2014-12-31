#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.63])
AC_INIT([fuego-net], [0.1], [two.oes@gmail.com])
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([src/fuegonetd.c])
AC_CONFIG_HEADERS([config.h])
LT_PREREQ([2.2])
LT_INIT([dlopen])
AC_CONFIG_MACRO_DIR([m4])


# Checks for programs.
PTHREAD_CFLAGS

AC_PROG_CC
AC_PROG_INSTALL
AM_PROG_CC_C_O

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([libconfig.h stdlib.h string.h strings.h unistd.h])

PKG_CHECK_MODULES([LIBCONFIGXX], [libconfig++ >= 1.4],,
       AC_MSG_ERROR([libconfig++ 1.4 or newer not found.]))


# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_FUNC_MALLOC

AC_OUTPUT

AC_OUTPUT([Makefile include/Makefile
		   man/Makefile
		   src/Makefile])


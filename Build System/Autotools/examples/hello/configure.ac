#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([hello], [1.0.0.0], [zjbhello@163.com])
AM_INIT_AUTOMAKE([hello],[1.0.0.0])
AC_CONFIG_SRCDIR([src/main/main.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
#AC_PROG_LIBTOOL
LT_INIT

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST

# Checks for library functions.

AC_OUTPUT(Makefile
	include/Makefile
	src/main/Makefile
	src/foo/Makefile
	src/bar/Makefile
	)

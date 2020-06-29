#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([hello], [1.0.0.5], [zjbhello@163.com])
#AM_INIT_AUTOMAKE(hello,1.0.0.0)
AC_CONFIG_SRCDIR([src/main/main.c])
AM_INIT_AUTOMAKE
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
# Checks for programs.
AC_PROG_CC
#AC_PROG_LIBTOOL
LT_INIT

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdio.h assert.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_SUBST([INCLUDE],[include])
AC_SUBST([SRCDIR],[src])
AC_SUBST([DOCDIR],[doc])
AC_SUBST([FOODIR],[foo])

m4_include(m4/deplibs.m4)

# Checks for library functions.

AC_OUTPUT(Makefile
	include/Makefile
	doc/Makefile
	src/Makefile
	src/main/Makefile
	src/thirdparty/Makefile
	src/foo/Makefile
	src/bar/Makefile
	src/play/Makefile
	src/play/play-1.0.pc
	)

#./configure --prefix=/home/xx --host=mips64el-loongson3a-linux-gnu

#../configure --prefix=/home/xroot --with-sysroot=/home/xroot --host=mips64el-loongson3a-linux-gnu LIBS=-L/home/xroot/usr/lib/ CPPFLAGS=-I/home/xroot/usr/include/

#64bit
#../configure --prefix=/home/xroot/ --host=mips64el-loongson3a-linux-gnu LIBS="-L/home/xroot/usr/lib64/" CPPFLAGS="-I/home/xroot/usr/include/"

#32bit
#../configure --prefix=/home/xroot/ --host=mips-linux-gnu CC=mips64el-loongson3a-n32-linux-gnu-gcc LIBS="-L/home/xroot/usr/lib/" CPPFLAGS="-I/home/xroot/usr/include"

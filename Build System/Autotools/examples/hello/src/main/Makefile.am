bin_PROGRAMS=hello
hello_SOURCES=main.c
hello_LDADD=$(top_srcdir)/src/foo/libfoo.a
LIBS=-lbar -lplay
AM_CPPFLAGS=-I$(top_srcdir)/include
hello_LDFLAGS=-L$(top_srcdir)/src/bar -L$(top_srcdir)/src/play

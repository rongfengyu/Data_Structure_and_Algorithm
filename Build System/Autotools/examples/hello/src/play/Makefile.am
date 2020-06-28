pkglib_LTLIBRARIES=libplay.la  ##凡是加上pkgxxx的，表示只是安装的时候加上包名 即pkglibdir=$(prefix)/lib/@PACKAGE@
libplay_la_SOURCES=play.c
AM_CPPFLAGS=-I$(top_srcdir)/include 

##因为play库是pkglib方式安装的，因此需要指定pkglibdir
pkgconfigdir=$(pkglibdir)/pkgconfig

##如果为了统一整个系统的pkgconfig，则指定为libdir，一般都在/lib/pkgconfig目录下
#pkgconfigdir=$(libdir)/pkgconfig

pkgconfig_DATA=play-1.0.pc 

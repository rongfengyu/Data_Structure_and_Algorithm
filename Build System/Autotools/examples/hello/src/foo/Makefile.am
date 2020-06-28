#noinst_LTLIBRARIES=libfoo.a  ##这一句和下一句都可以生成静态库，只是这一句是利用libtool来生成静态库，下一句不使用libtool速度更快
noinst_LIBRARIES=libfoo.a  ##静态库不用发布，noinst
libfoo_a_SOURCES=foo.c
AM_CPPFLAGS=-I$(top_srcdir)/include   ##$(top_srcdir)变量对整个工程来说其绝对意义是不变的，即始终是顶层目录位置 

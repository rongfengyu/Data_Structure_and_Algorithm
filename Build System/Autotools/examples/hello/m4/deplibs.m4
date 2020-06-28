AC_MSG_CHECKING(for zlib support)
AC_ARG_WITH(zlib, AC_HELP_STRING([--with-zlib],[Enable zlib support for mod_compress]),
    [WITH_ZLIB=$withval],[WITH_ZLIB=yes])
AC_MSG_RESULT([$WITH_ZLIB])

if test "$WITH_ZLIB" != "no"; then
  AC_CHECK_LIB(z, deflate, [
    AC_CHECK_HEADERS([zlib.h],[
      Z_LIB=-lz
      AC_DEFINE([HAVE_LIBZ], [1], [libz])
      AC_DEFINE([HAVE_ZLIB_H], [1])
    ])
  ])
  if test x$Z_LIB = x; then
     AC_MSG_ERROR([zlib-headers and/or libs where not found, install them or build with --without-zlib])
  fi
fi
AC_SUBST(Z_LIB)

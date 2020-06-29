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

AC_MSG_CHECKING(for pcre support)
AC_ARG_WITH(pcre, AC_HELP_STRING([--with-pcre],[Enable pcre support for mod_compress]),
    [WITH_PCRE=$withval],[WITH_PCRE=yes])
AC_MSG_RESULT([$WITH_PCRE])

if test "$WITH_PCRE" != "no"; then
  AC_CHECK_LIB(pcre, pcre_malloc, [
    AC_CHECK_HEADERS([pcre.h],[
      PCRE_LIB=-lpcre
      AC_DEFINE([HAVE_LIBPCRE], [1], [libpcre])
      AC_DEFINE([HAVE_PCRE_H], [1])
    ])
  ])
  if test x$PCRE_LIB = x; then
     AC_MSG_ERROR([pcre-headers and/or libs where not found, install them or build with --without-pcre])
  fi
fi
AC_SUBST(PCRE_LIB)

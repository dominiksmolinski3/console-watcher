
AC_INIT([console-watcher], [1.0], [your-email@example.com])
AM_INIT_AUTOMAKE([foreign])

AC_PROG_CXX
AC_PROG_CC

PKG_CHECK_MODULES([X11], [x11])

CXXFLAGS="${CXXFLAGS} ${X_CFLAGS}"
LDFLAGS="${LDFLAGS} ${X_LIBS}"

AC_CONFIG_SRCDIR([main.cpp])
AC_CONFIG_HEADERS([config.h])

AC_CONFIG_FILES([Makefile])

AC_OUTPUT

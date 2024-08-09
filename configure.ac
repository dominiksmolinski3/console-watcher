# Initialize the Autoconf configuration
AC_INIT([console-watcher], [1.0], [your-email@example.com])
AM_INIT_AUTOMAKE([foreign])

# Check for required packages
AC_PROG_CXX
AC_PROG_CC

# Check for X11 libraries
PKG_CHECK_MODULES([X11], [x11])

# Enable languages
AM_LANG([C++])

# Set compiler flags
AC_CXXFLAGS([${X_CFLAGS}])
AC_LDFLAGS([${X_LIBS}])

# Set the installation directories
AC_CONFIG_SRCDIR([main.cpp])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile])

# Generate the configuration files
AC_OUTPUT

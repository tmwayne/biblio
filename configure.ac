#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
# If the project has a wildly different structure, use autoscan
# to generate an appropriate configure.ac

AC_PREREQ([2.69])
AC_INIT([Biblio], [0.5], [tylerwayne3@gmail.com])
AM_INIT_AUTOMAKE([subdir-objects])

LT_PREREQ([2.2.])
LT_INIT([dlopen])

AC_CONFIG_SRCDIR([src/biblio.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h dlfcn.h libpq-fe.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_SEARCH_LIBS([dlopen])

AC_CONFIG_FILES([
  Makefile
  src/Makefile
  lib/Makefile
  test/Makefile])

AC_OUTPUT

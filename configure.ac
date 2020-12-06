#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
# If the project has a wildly different structure, use autoscan
# to generate an appropriate configure.ac

AC_PREREQ([2.69])
AC_INIT([Biblio], [0.5], [tylerwayne3@gmail.com])
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([src/biblio.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
# TODO: Update this with the actual headers
AC_CHECK_HEADERS([stdlib.h libpq-fe.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile src/Makefile tests/Makefile])
AC_OUTPUT

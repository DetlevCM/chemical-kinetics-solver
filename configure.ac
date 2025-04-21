#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.72])
AC_INIT([Chelical Kinetics Solver], [VERSION], [BUG-REPORT-ADDRESS])

# auxilliary directory
#AC_CONFIG_AUX_DIR([config])

# defines header and source code
AC_CONFIG_SRCDIR([src/Main.cpp])
#AC_CONFIG_HEADERS([include/Headers.hpp])

# Checks for programs/compilers:
AC_PROG_CXX
# https://www.gnu.org/software/autoconf/manual/autoconf-2.67/html_node/Fortran-Compiler.html#Fortran-Compiler
AC_PROG_FC

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_STRTOD
AC_CHECK_FUNCS([pow strtol])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT

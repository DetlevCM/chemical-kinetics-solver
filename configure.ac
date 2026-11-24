#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.72])
AC_INIT([Chemical Kinetics Solver], [VERSION], [BUG-REPORT-ADDRESS])

# auxilliary directory
#AC_CONFIG_AUX_DIR([config])

# defines header and source code
AC_CONFIG_SRCDIR([src/main.cpp])
#AC_CONFIG_HEADERS([src/main.h])

# Checks for programs/compilers:
AC_PROG_CXX
# https://www.gnu.org/software/autoconf/manual/autoconf-2.67/html_node/Fortran-Compiler.html#Fortran-Compiler
AC_PROG_FC

# Checks for libraries.

# Checks for header files.

AX_CXX_COMPILE_STDCXX_11([ext], [mandatory])
AX_CXX_COMPILE_STDCXX_14([ext], [optional])
AX_CXX_COMPILE_STDCXX_17([ext], [optional])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADERS(vector math.h)
AC_CHECK_HEADER_STDBOOL

AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_STRTOD
#AC_CHECK_FUNCS(pow stoi stod)

AC_CONFIG_FILES([Makefile])
AC_OUTPUT

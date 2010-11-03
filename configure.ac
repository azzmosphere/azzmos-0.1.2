AC_INIT( [azzmos], [0.1.2], [azzmosphere@gmail.com])
AM_CONFIG_HEADER(config.h)

dnl Package information
AC_CONFIG_SRCDIR(src/azzmos.c)
AM_INIT_AUTOMAKE

dnl program checks
AC_PROG_CC
AC_LANG_C
AM_PROG_CC_C_O
AC_PROG_LIBTOOL


AC_CONFIG_FILES([
  Makefile
  include/Makefile
  include/azzmos/Makefile
  lib/Makefile
  src/Makefile
  test/Makefile
])

# configure the m4 directories
AC_CONFIG_MACRO_DIR([m4])


dnl library checks
LIBCURL_CHECK_CONFIG( , [ 7.10.03 ], , AC_MSG_ERROR( [ libCurl version 7.10.03 or above is required ] ))
AX_PATH_LIB_PCRE([], [AC_MSG_ERROR([pcre required to build])])

dnl The PG debugger should be required here,  so before updating 
dnl a PostgresQL version ensure the the pldebugger works with it.
AX_LIB_POSTGRESQL([8.2.6])
if test -z ${POSTGRESQL_CFLAGS}; then
	AC_MSG_ERROR([PostgreSQL >= 8.4.3 is required])
fi

dnl header file checks

dnl type checks

dnl structures checks 

dnl compiler character checks

dnl library function checks

dnl system services checks

dnl AC_SUBST(PACKAGE_STRING)

AC_OUTPUT

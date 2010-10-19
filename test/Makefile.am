SOURCES = CuTest.c CuTest.h
INCLUDES = @POSTGRESQL_CFLAGS@ -I$(top_srcdir)/include
AM_CFLAGS = -I$(top_srcdir)/src
AM_LDFLAGS = @POSTGRESQL_LDFLAGS@ \
             @LIBCURL@  \
             -L$(top_builddir)/lib -lazzmos
test_uriobj_SOURCES     = test_uriobj.c $(SOURCES)
test_uriparse_SOURCES   = test_uriparse.c $(SOURCES)
test_urinorm_SOURCES    = test_urinorm.c $(SOURCES)
test_uriresolve_SOURCES = test_uriresolve.c $(SOURCES) \
			  $(top_srcdir)/src/uriresolve.c
test_urilist_SOURCES    = test_urilist.c $(SOURCES)
check_PROGRAMS = test_uriobj \
		 test_uriparse \
		 test_urinorm \
		 test_uriresolve \
		 test_urilist
TESTS = test_uriobj \
	test_uriparse \
	test_urinorm \
	test_uriresolve \
	test_urilist

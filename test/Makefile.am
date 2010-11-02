#
# test data is sent to the compiler as __TEST_DATA__, this MACRO 
# can be (should be) used to contain all static files that are 
# required for testing.
# 
#
SOURCES = CuTest.c CuTest.h
INCLUDES = @POSTGRESQL_CFLAGS@ -I$(top_srcdir)/include
AM_CFLAGS = -I$(top_srcdir)/src -D__TEST_DATA__=\"$(top_srcdir)/data\"
AM_LDFLAGS = @POSTGRESQL_LDFLAGS@ \
             @LIBCURL@  \
             -L$(top_builddir)/lib -lazzmos
test_uriobj_SOURCES     = test_uriobj.c $(SOURCES)
test_uriparse_SOURCES   = test_uriparse.c $(SOURCES)
test_urinorm_SOURCES    = test_urinorm.c $(SOURCES)
test_uriresolve_SOURCES = test_uriresolve.c $(SOURCES)
test_urilist_SOURCES    = test_urilist.c $(SOURCES)
test_httpheaders_SOURCES = test_httpheaders.c $(SOURCES)
check_PROGRAMS = test_uriobj \
		 test_uriparse \
		 test_urinorm \
		 test_uriresolve \
		 test_urilist \
		 test_httpheaders
TESTS = test_uriobj \
	test_uriparse \
	test_urinorm \
	test_uriresolve \
	test_urilist \
	test_httpheaders

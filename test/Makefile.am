#
# test data is sent to the compiler as __TEST_DATA__, this MACRO 
# can be (should be) used to contain all static files that are 
# required for testing.
# 
#
SOURCES = CuTest.c CuTest.h
INCLUDES = @POSTGRESQL_CFLAGS@ -I$(top_srcdir)/include

#
# Set the data directory MACRO,  this is expaned in many tests but it
# is up to the test to know which file that it is looking for.
AM_CFLAGS = -I$(top_srcdir)/src -D__TEST_DATA__=\"$(top_srcdir)/data\"
AM_LDFLAGS = @POSTGRESQL_LDFLAGS@ \
             @LIBCURL@  \
             -L$(top_builddir)/lib -lazzmos

#
# Create the source files for each test.
test_uriobj_SOURCES     = test_uriobj.c $(SOURCES)
test_uriparse_SOURCES   = test_uriparse.c $(SOURCES)
test_urinorm_SOURCES    = test_urinorm.c $(SOURCES)
test_uriresolve_SOURCES = test_uriresolve.c $(SOURCES)
test_urilist_SOURCES    = test_urilist.c $(SOURCES)
test_httpheaders_SOURCES = test_httpheaders.c $(SOURCES)
test_htmlpage_SOURCES    = test_htmlpage.c $(SOURCES)

#
# the following program will be used for unit testing
check_PROGRAMS = test_uriobj \
		 test_uriparse \
		 test_urinorm \
		 test_uriresolve \
		 test_urilist \
		 test_httpheaders \
		 test_htmlpage
#
# Run the following unit tests
TESTS = test_uriobj \
	test_uriparse \
	test_urinorm \
	test_uriresolve \
	test_urilist \
	test_httpheaders \
	test_htmlpage

/*
 * =====================================================================================
 *
 *       Filename:  test_uriobj.c
 *
 *    Description:  tests the algoritms of uriobj.c
 *
 *        Version:  1.0
 *        Created:  19/09/2010 12:03:01
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Aaron Spiteri
 *        Company:  
 *
 * =====================================================================================
 */
#include <CuTest.h>
#include <azzmos-fe.h>

uriobj_t *uri;

void
test_uri_alloc(CuTest *tc)
{
	uri = uri_alloc();
	CuAssertPtrNotNull(tc, uri);
}

void
test_uri_clone(CuTest *tc)
{
	int actual = 0;
	uriobj_t *ref;
	uri->uri_scheme = strdup("http");
	uri->uri_host   = strdup("www.google.com.au");
	uri_clone(&ref, uri);
	actual  = strcmp(ref->uri_scheme,"http");
	actual += strcmp(ref->uri_host,"www.google.com.au");
	CuAssertIntEquals(tc,0,actual);
	
}

void
test_uri_free(CuTest *tc)
{
	uri_free(uri);
	CuAssertIntEquals(tc, 1, 1);
}

CuSuite *
GetSuite()
{
	CuSuite *suite = CuSuiteNew();
	SUITE_ADD_TEST( suite, test_uri_alloc);
	SUITE_ADD_TEST( suite, test_uri_clone);
	SUITE_ADD_TEST( suite, test_uri_free);
}

int 
main()
{
	CuSuite  *suite  = CuSuiteNew();
	CuString *output = CuStringNew();
	CuSuiteAddSuite( suite, GetSuite());
	CuSuiteRun(suite);
	CuSuiteSummary( suite, output);
	fprintf( stdout, "%s\n", output->buffer);
	exit(suite->failCount);
}


/*
 * =====================================================================================
 *
 *       Filename:  test_urinorm.c
 *
 *    Description:  tests the algoritms of uriobj.c
 *
 *        Version:  1.0
 *        Created:  20/09/2010 12:03:01
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Aaron Spiteri
 *        Company:  
 *
 * =====================================================================================
 */
#include <CuTest.h>
#include <azzmos/uriresolve.h>

/*=====================================================================================
 * IP resolution is dodgy at best.  That is we can not garantee that the system will
 * allways all IP's for a given URI since it will resolve them of what the DNS and
 * the load balancer decide is the amount of IPs a given FQP has.  Therefore this 
 * test should be modified to suite.
 * ====================================================================================
 */
#define IP_TEST_IP "192.0.32.10"

regexpr_t *re;

void
test_uri_resolve_1(CuTest *tc)
{
	char *href = strdup("http://www.google.com/");
	uriobj_t *uri = uri_alloc();
	int err = 0;
	re = (regexpr_t *) malloc( sizeof(regexpr_t));
	uri_init_regex(re);
	uri_parse(&uri,re,href);
	uri_parse_auth(&uri);
	err = uri_resolve(&uri);
    printf("ip = '%s'\n", uri->uri_ip->ip_addr);
    uri_free(uri);
	CuAssertIntEquals(tc,0,err);    
}

/*=====================================================================================
 * RFC 2606 madates that the second level domains .example.(com|net|org) must exist.
 * ====================================================================================
 */
void
test_uri_resolve_2(CuTest *tc)
{
	char *href = strdup("http://example.com/");
	uriobj_t *uri = uri_alloc();
	int err = 0;
    uri_parse(&uri,re,href);
	uri_parse_auth(&uri);
	err = uri_resolve(&uri);
    printf("ip    = '%s'\n", uri->uri_ip->ip_addr);
    printf("cname = '%s'\n", uri->uri_ip->ip_cname);
	CuAssertPtrNotNull(tc,uri->uri_ip->ip_addr);  
    uri_free(uri);
}

/*=====================================================================================
 * RFC 2606 madates that the first level domains .test, .example, .invalid and 
 * .localhost can not be registered.  It has become common practice for servers
 * to use .localhost to reference themselves, therefore for testing handling of
 * invalid domains this test code uses the domain ".invalid"
 * ====================================================================================
 */
void
test_uri_resolve_3(CuTest *tc)
{
	char *href = strdup("http://invalid/");
	uriobj_t *uri = uri_alloc();
	int err = 0;
    uri_parse(&uri,re,href);
	uri_parse_auth(&uri);
	err = uri_resolve(&uri);
    printf("err    = '%d'\n", err); 
    printf("errstr = '%s'\n",gai_strerror(err));
    uri_free(uri);
}


void
test_ref_resolve_1(CuTest *tc)
{
	char *href = strdup("http://www.example.com/"),
         *result;
	uriobj_t *uri = uri_alloc(),
             *ref = uri_alloc();	
    uri_parse(&uri,re,href);
	uri_parse_auth(&uri);
	uri_resolve(&uri);
	ref = ref_resolve("path/to/uri.html",uri,re,false);
    result = strdup(ref->uri_path);
    uri_free(uri);
    uri_free(ref);
	CuAssertStrEquals(tc,result,"/path/to/uri.html");
}

void
test_ref_resolve_2(CuTest *tc)
{
	char *href = strdup("http://www.example.com/some/dir/to/path"),
         *result;
	uriobj_t *uri = uri_alloc(),
             *ref = uri_alloc();
	uri_parse(&uri,re,href);
	uri_parse_auth(&uri);
	uri_resolve(&uri);
	ref = ref_resolve("path/to/uri.html",uri,re,false);
    result = strdup(ref->uri_path);
    uri_free(ref);
    uri_free(uri);
    CuAssertStrEquals(tc,"/some/dir/to/path/to/uri.html",result);
}

void
test_ref_resolve_3(CuTest *tc)
{
	char *href = strdup("http://www.example.com/"),
         *result = NULL;
	uriobj_t *uri = uri_alloc(),
             *ref = uri_alloc();
	uri_parse(&uri,re,href);
	uri_parse_auth(&uri);
	uri_resolve(&uri);
	ref = ref_resolve("path/to/uri.html",uri,re,false);
    result = strdup(ref->uri_path);
    uri_free(ref);
    uri_free(uri);
    CuAssertStrEquals(tc,"/path/to/uri.html",result);
}

void
test_uri_trans_ref1(CuTest *tc)
{
	char *href = strdup("http://www.example.com/some/dir/to/path"),
         *result;
	uriobj_t *uri = uri_alloc(),
	         *ref = uri_alloc(),
			 *trans;
	uri_parse(&uri,re,href);
	uri_parse_auth(&uri);
	uri_parse(&ref,re,"path/to/uri.html");
	trans = uri_trans_ref(ref,uri,false);
	uri_resolve(&uri);
    result = strdup(trans->uri_path);
    uri_free(uri);
    uri_free(trans);
    uri_free(ref);
	CuAssertStrEquals(tc, result,"/some/dir/to/path/to/uri.html");
}


CuSuite *
GetSuite()
{
	CuSuite *suite = CuSuiteNew();
	SUITE_ADD_TEST( suite, test_uri_resolve_1);
	SUITE_ADD_TEST( suite, test_uri_resolve_2);
    SUITE_ADD_TEST( suite, test_uri_resolve_3);
	//SUITE_ADD_TEST( suite, test_ref_resolve_1);
	//SUITE_ADD_TEST( suite, test_ref_resolve_2);
	//SUITE_ADD_TEST( suite, test_ref_resolve_3);
	//SUITE_ADD_TEST( suite, test_uri_trans_ref1);
    return suite;
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

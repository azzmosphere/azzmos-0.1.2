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
#include <uriresolve.h>

regexpr_t *re;

void
test_uri_resolve_1(CuTest *tc)
{
	char *href = strdup("http://www.example.com/");
	uriobj_t *uri = uri_alloc();
	int err = 0;
	re = (regexpr_t *) malloc( sizeof(regexpr_t));
	uri_init_regex(re);
	uri_parse(&uri,re,href);
	uri_parse_auth(&uri);
	err = uri_resolve(&uri);
	CuAssertIntEquals(tc,0,err);
}

void
test_uri_resolve_2(CuTest *tc)
{
	char *href = strdup("http://www.example.com/"),
		 *ip,
		 *ipex = strdup("184.106.31.181");
	uriobj_t *uri = uri_alloc();
	int err = 1;
	struct sockaddr_in *addr_in;
	struct addrinfo *addr;
	uri_parse(&uri,re,href);
	uri_parse_auth(&uri);
	uri_resolve(&uri);
	addr = uri->uri_ip;
	while( addr != NULL){
		addr_in = (struct sockaddr_in *)addr->ai_addr;
		ip = malloc(addr->ai_addrlen);
		//bzero(ip,addr->ai_addrlen);
		strncpy(ip, inet_ntop(addr->ai_family,
		               &addr_in->sin_addr,
					   ip,
					   addr->ai_addrlen), (addr->ai_addrlen - 1));
		err = strcmp(ip,ipex);
		printf("ip = '%s' - len = '%d' addrlen = %d\n", ip, strlen(ip), addr->ai_addrlen);
		free(ip);
		if(err == 0 ){
			break;
		}
		addr = addr->ai_next;
	}
	CuAssertIntEquals(tc,0,err);
}

void
test_ref_resolve_1(CuTest *tc)
{
	char *href = strdup("http://www.example.com/");
	uriobj_t *uri = uri_alloc(),
	         *ref = uri_alloc(),
			 *trans;
	uri_parse(&uri,re,href);
	uri_parse_auth(&uri);
	uri_resolve(&uri);
	uri_parse(&ref,re,"path/to/uri.html");
	ref = ref_resolve("path/to/uri.html",uri,re,false);
	printf("ref->uri_path = '%s'\n", ref->uri_path);
}

void
test_ref_resolve_2(CuTest *tc)
{
	char *href = strdup("http://www.example.com/some/dir/to/path");
	uriobj_t *uri = uri_alloc(),
	         *ref = uri_alloc();
	uri_parse(&uri,re,href);
	uri_parse_auth(&uri);
	uri_resolve(&uri);
	uri_parse(&ref,re,"path/to/uri.html");
	ref = ref_resolve("path/to/uri.html",uri,re,false);
	printf("ref->uri_path = '%s'\n", ref->uri_path);
}

void
test_ref_resolve_3(CuTest *tc)
{
	char *href = strdup("http://www.example.com/");
	uriobj_t *uri = uri_alloc(),
	         *ref = uri_alloc(),
			 *trans;
	uri_parse(&uri,re,href);
	uri_parse_auth(&uri);
	uri_resolve(&uri);
	uri_parse(&ref,re,"path/to/uri.html");
	ref = ref_resolve("path/to/uri.html",uri,re,false);
	printf("ref->uri_path = '%s'\n", ref->uri_path);
}

void
test_uri_trans_ref1(CuTest *tc)
{
	char *href = strdup("http://www.example.com/some/dir/to/path"),
	     *expect = strdup("/some/dir/to/path/to/uri.html");
	uriobj_t *uri = uri_alloc(),
	         *ref = uri_alloc(),
			 *trans;
	uri_parse(&uri,re,href);
	uri_parse_auth(&uri);
	uri_parse(&ref,re,"path/to/uri.html");
	trans = uri_trans_ref(ref,uri,false);
	uri_resolve(&uri);
	CuAssertStrEquals(tc, trans->uri_path,"/some/dir/to/path/to/uri.html");
}


CuSuite *
GetSuite()
{
	CuSuite *suite = CuSuiteNew();
	SUITE_ADD_TEST( suite, test_uri_resolve_1);
	SUITE_ADD_TEST( suite, test_uri_resolve_2);
	//SUITE_ADD_TEST( suite, test_ref_resolve_1);
	//SUITE_ADD_TEST( suite, test_ref_resolve_2);
	//SUITE_ADD_TEST( suite, test_ref_resolve_3);
	SUITE_ADD_TEST( suite, test_uri_trans_ref1);
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

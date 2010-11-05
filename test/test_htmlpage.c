/*
 * =====================================================================================
 *
 *       Filename:  test_htmlpage.c
 *
 *    Description:  testsa htmlpage object and functions
 *
 *        Version:  1.0
 *        Created:  03/11/2010 22:39:01
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
#define HEADERS_TXT __TEST_DATA__ "/headers.txt"

htmlpage_t *html;

/* =====================================================================================
 * httpheader_alloc should be the first ran as it will allocate the html object, if this
 * fails all other tests are bound to fail.
 * =====================================================================================
 */ 
void
test_httpheaders_alloc_1(CuTest *tc)
{
    html = htmlpage_alloc();
    CuAssertPtrNotNull(tc, html);
}
void
test_html_set_hdr_1(CuTest *tc)
{
    int error = 0;
    FILE *stream = fopen(HEADERS_TXT,"r");
    
    error = html_set_hdr(&html,stream);
    fclose(stream);
    CuAssertIntEquals(tc,0,error);
}

void
test_html_set_url_1(CuTest *tc)
{
    uriobj_t *url = uri_alloc();
    const char *href = "http://www.example.com/";
    regexpr_t *re = (regexpr_t *) malloc( sizeof(regexpr_t));
    int err = 0;
    
    uri_init_regex(re);
    uri_parse(&url,re,href);
    uri_parse_auth(&url);
    err = uri_resolve(&url);
    err = html_set_url(&html, &url);
    CuAssertIntEquals(tc,0,err);
    printf("set break point");
}

/* =====================================================================================
 * httpheader_free should be the last test called and must be called either last or not
 * at all, as it will free the object and any data associated with it.
 * =====================================================================================
 */ 
void
test_httpheaders_free_1(CuTest *tc)
{
    htmlpage_free(html);
}

CuSuite *
GetSuite()
{
	CuSuite *suite = CuSuiteNew();
	SUITE_ADD_TEST( suite, test_httpheaders_alloc_1);
    SUITE_ADD_TEST( suite, test_html_set_hdr_1);
    SUITE_ADD_TEST( suite, test_html_set_url_1);
    SUITE_ADD_TEST( suite, test_httpheaders_free_1);
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

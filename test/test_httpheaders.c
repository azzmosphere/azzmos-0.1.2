/*
 * =====================================================================================
 *
 *       Filename:  test_httpheaders.c
 *
 *    Description:  tests  the construction and tear down of http headers.
 *
 *        Version:  1.0
 *        Created:  29/10/2010 22:39:01
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
#include <sys/param.h>
#define HEADERS_TXT __TEST_DATA__ "/headers.txt"

httpheaders_t *hd;

void
test_httpheaders_alloc_1(CuTest *tc)
{
    hd = httpheaders_alloc();
    CuAssertPtrNotNull(tc, hd);
}

void
test_httpheaders_init_1(CuTest *tc)
{
    int err = 0;
    err = httpheaders_init(&hd);
    CuAssertIntEquals(tc, err, 0);
}

/*=====================================================================================
 * test_httpheaders_build_1
 *
 * open the headers stream and assign the headers.  This function is used for later 
 * tests so its position is important.
 *=====================================================================================
 */
void 
test_httpheaders_build_1(CuTest *tc)
{
    int err = 0;
    FILE *hstream;
    char *cwd = malloc((MAXPATHLEN + 1));
    
    cwd = getcwd(cwd,MAXPATHLEN);
    printf("current path = '%s'\n", cwd);
    printf("using headers from file - '%s'\n",HEADERS_TXT);
    hstream = fopen(HEADERS_TXT,"r");
    if(hstream == NULL){
        printf("could not open file stream - %s\n", strerror(errno));
        CuAssertIntEquals(tc,0,1);
        return;
    }
    err = httpheaders_build(&hd, hstream);
    fclose(hstream);
    free(cwd);
    CuAssertIntEquals(tc,err,0);
}

void 
test_httpheader_get_val_1(CuTest *tc)
{
    char *value = httpheader_get_val(hd,"Date");
    CuAssertStrEquals(tc,value,"Sat, 05 Jun 2010 13:36:39 GMT");
    safe_free(value);
}

void 
test_httpheader_get_val_2(CuTest *tc)
{
    char *value = httpheader_get_val(hd,"Server");
    CuAssertStrEquals(tc,value,"Apache/1.3.29");
    safe_free(value);
}

void 
test_httpheader_get_val_3(CuTest *tc)
{
    char *value = httpheader_get_val(hd,"ETag");
    CuAssertStrEquals(tc,value,"\"da4e3e-f77c-4c0a52c5\"");
    safe_free(value);
}

void 
test_httpheaders_free_1(CuTest *tc)
{
    httpheaders_free(hd);
}

CuSuite *
GetSuite()
{
	CuSuite *suite = CuSuiteNew();
	SUITE_ADD_TEST( suite, test_httpheaders_alloc_1);
    SUITE_ADD_TEST( suite, test_httpheaders_init_1);
    SUITE_ADD_TEST( suite, test_httpheaders_build_1);
    SUITE_ADD_TEST( suite, test_httpheader_get_val_1);
    SUITE_ADD_TEST( suite, test_httpheader_get_val_2);
    SUITE_ADD_TEST( suite, test_httpheader_get_val_3);
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

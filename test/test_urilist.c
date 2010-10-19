/*
 * =====================================================================================
 *
 *       Filename:  test_urilist.c
 *
 *    Description:  test the list functions of the URI object
 *
 *        Version:  1.0
 *        Created:  19/10/2010 22:47
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

regexpr_t *re;
uriobj_t  *list;

void
test_urilist_1(CuTest *tc)
{
    re = (regexpr_t *) malloc( sizeof(regexpr_t));
    uri_init_regex(re);
    uriobj_t *uri = uri_alloc(), *first;
    list = uri_alloc();
    char *fqp1 = "http://www.ics.uci.edu/pub/ietf/uri/#Related",
         *fqp2 = "http://www.example.com/a/path/to/uri";
    uri_parse(&list, re, fqp1);
    uri_parse(&uri, re, fqp2);
    INIT_LIST_HEAD(&list->uri_list);
    list_add(&uri->uri_list, &list->uri_list);
}

void
test_urilist_2(CuTest *tc)
{
    struct list_head *pos;
    uriobj_t *uri;
    
    printf("\n\ntesting list 1\n");
    printf("auth = '%s'\n", list->uri_auth);
    list_for_each(pos, &list->uri_list){
        uri = list_entry(pos, uriobj_t, uri_list);
        printf("auth = '%s'\n", uri->uri_auth);
    }
}

void
test_urilist_3(CuTest *tc)
{
    struct list_head *pos;
    uriobj_t *uri = uri_alloc();
    
    printf("\n\ntesting list 2\n");
    uri_parse(&uri, re, "http://www.google.com/");
    list_add(&uri->uri_list, &list->uri_list);
    printf("auth = '%s'\n", list->uri_auth);
    list_for_each(pos, &list->uri_list){
        uri = list_entry(pos, uriobj_t, uri_list);
        printf("auth = '%s'\n", uri->uri_auth);
    }
}

CuSuite *
GetSuite()
{
	CuSuite *suite = CuSuiteNew();
	SUITE_ADD_TEST( suite, test_urilist_1);
    SUITE_ADD_TEST( suite, test_urilist_2);
    SUITE_ADD_TEST( suite, test_urilist_3);
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

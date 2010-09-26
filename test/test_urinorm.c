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
#include <azzmos-fe.h>

void
test_uri_norm_scheme_1(CuTest *tc)
{
	char *scheme = strdup("http");
	int result = uri_norm_scheme(&scheme);
	CuAssertIntEquals(tc, result, 0);
}

void
test_uri_norm_scheme_2(CuTest *tc)
{
	char *scheme = strdup("HTTps");
	int result = uri_norm_scheme(&scheme);
	CuAssertStrEquals(tc, scheme, "https");
}

void
test_uri_norm_scheme_3(CuTest *tc)
{
	char *scheme = strdup("HTTps ");
	int result = uri_norm_scheme(&scheme);
	CuAssertIntEquals(tc, result, EILSEQ);
}

void
test_uri_norm_scheme_4(CuTest *tc)
{
	char *scheme = NULL;
	int result = uri_norm_scheme(&scheme);
	CuAssertIntEquals(tc, result, ENOATTR);
}

void
test_uri_norm_scheme_5(CuTest *tc)
{
	char *scheme = strdup("4http");
	int result = uri_norm_scheme(&scheme);
	CuAssertIntEquals(tc, result, EILSEQ);
}

void
test_uri_norm_reg_name_1(CuTest *tc)
{
	char *host = strdup("www.example.com");
	int result = uri_norm_reg_name(&host);
	CuAssertIntEquals(tc, result, 0);
}

void
test_uri_norm_reg_name_2(CuTest *tc)
{
	char *host = strdup("www.eXample.COM");
	int result = uri_norm_reg_name(&host);
	CuAssertStrEquals(tc, host, "www.example.com");
}

void
test_uri_norm_reg_name_3(CuTest *tc)
{
	char *host = strdup("www.eXample%a1.COM");
	int result = uri_norm_reg_name(&host);
	CuAssertStrEquals(tc, host, "www.example%A1.com");
}

void
test_uri_norm_reg_name_4(CuTest *tc)
{
	char *host = strdup("www .eXample%a1.COM");
	int result = uri_norm_reg_name(&host);
	CuAssertIntEquals(tc, result, EILSEQ);
}

void
test_uri_norm_reg_name_5(CuTest *tc)
{
	char *host = strdup("12eXample%a1.COM");
	int result = uri_norm_reg_name(&host);
	CuAssertStrEquals(tc, host, "12example%A1.com");
}

void
test_uri_norm_reg_name_6(CuTest *tc)
{
	char *host = strdup("12eXample%a.COM");
	int result = uri_norm_reg_name(&host);
	CuAssertIntEquals(tc, result, EILSEQ);
}

void
test_uri_norm_reg_name_7(CuTest *tc)
{
	char *host = strdup("12eXample%a");
	int result = uri_norm_reg_name(&host);
	CuAssertIntEquals(tc, result, EILSEQ);
}

void
test_uri_norm_reg_name_8(CuTest *tc)
{
	char *host = strdup("12eXample%a1");
	int result = uri_norm_reg_name(&host);
	CuAssertIntEquals(tc, result, 0);
}

void
test_uri_norm_port_1(CuTest *tc)
{
	char *port = strdup("8080");
	int result = uri_norm_port(&port);
	CuAssertIntEquals(tc, result, 0);
}

void
test_uri_norm_port_2(CuTest *tc)
{
	char *port = NULL;
	int result = uri_norm_port(&port);
	CuAssertIntEquals(tc, result, 0);
}

void
test_uri_norm_port_3(CuTest *tc)
{
	char *port = strdup(" 80");
	int result = uri_norm_port(&port);
	CuAssertIntEquals(tc, result, EILSEQ);
}

void
test_uri_norm_path_1(CuTest *tc)
{
	char *path = NULL;
	int result = uri_norm_path(&path);
	CuAssertIntEquals(tc, result, EINVAL);
}

void
test_uri_norm_path_2(CuTest *tc)
{
	char *path = strdup("/path/with/illegal/pchar/%G2/test.html");
	int result = uri_norm_path(&path);
	CuAssertIntEquals(tc, result, EILSEQ);
}

void
test_uri_norm_path_3(CuTest *tc)
{
	char *path = strdup("/path/with/illegal/pchar/%c2/test.html");
	int result = uri_norm_path(&path);
	CuAssertStrEquals(tc, path, "/path/with/illegal/pchar/%C2test.html");
}

void
test_uri_norm_path_4(CuTest *tc)
{
	char *path = strdup("/path/with/illegal/pchar/%C2/ test.html");
	int result = uri_norm_path(&path);
	CuAssertIntEquals(tc, result, EILSEQ);
}

void
test_uri_norm_ipv4_1(CuTest *tc)
{
	char *ip = strdup("192.168.1.1");
	int result = uri_norm_ipv4(&ip);
	CuAssertIntEquals(tc, result, 0);
}

void
test_uri_norm_ipv4_2(CuTest *tc)
{
	char *ip = NULL;
	int result = uri_norm_ipv4(&ip);
	CuAssertIntEquals(tc, result, EINVAL);
}

void
test_uri_norm_ipv4_3(CuTest *tc)
{
	char *ip = strdup("192.168..1");
	int result = uri_norm_ipv4(&ip);
	CuAssertIntEquals(tc, result, EILSEQ);
}

void
test_uri_norm_ipv4_4(CuTest *tc)
{
	char *ip = strdup("192.168.1.1.1");
	int result = uri_norm_ipv4(&ip);
	CuAssertIntEquals(tc, result, ERANGE);
}

void
test_uri_norm_ipv4_5(CuTest *tc)
{
	char *ip = strdup("192.168.01.1");
	int result = uri_norm_ipv4(&ip);
	CuAssertIntEquals(tc, result, EILSEQ);
}

void
test_uri_norm_ipv4_6(CuTest *tc)
{
	char *ip = strdup("192.168.300.1");
	int result = uri_norm_ipv4(&ip);
	CuAssertIntEquals(tc, result, EILSEQ);
}

void
test_uri_norm_ipv4_7(CuTest *tc)
{
	char *ip = strdup("192.168.a1.1");
	int result = uri_norm_ipv4(&ip);
	CuAssertIntEquals(tc, result, EILSEQ);
}

CuSuite *
GetSuite()
{
	CuSuite *suite = CuSuiteNew();
	SUITE_ADD_TEST( suite, test_uri_norm_scheme_1);
	SUITE_ADD_TEST( suite, test_uri_norm_scheme_2);
	SUITE_ADD_TEST( suite, test_uri_norm_scheme_3);	
	SUITE_ADD_TEST( suite, test_uri_norm_scheme_4);
	SUITE_ADD_TEST( suite, test_uri_norm_scheme_5);
	SUITE_ADD_TEST( suite, test_uri_norm_reg_name_1);
	SUITE_ADD_TEST( suite, test_uri_norm_reg_name_2);
	SUITE_ADD_TEST( suite, test_uri_norm_reg_name_3);
	SUITE_ADD_TEST( suite, test_uri_norm_reg_name_4);
	SUITE_ADD_TEST( suite, test_uri_norm_reg_name_5);
	SUITE_ADD_TEST( suite, test_uri_norm_reg_name_6);
	SUITE_ADD_TEST( suite, test_uri_norm_reg_name_7);
	SUITE_ADD_TEST( suite, test_uri_norm_reg_name_8);
	SUITE_ADD_TEST( suite, test_uri_norm_port_1);
	SUITE_ADD_TEST( suite, test_uri_norm_port_2);
	SUITE_ADD_TEST( suite, test_uri_norm_port_3);
	SUITE_ADD_TEST( suite, test_uri_norm_path_1);
	SUITE_ADD_TEST( suite, test_uri_norm_path_2);
	SUITE_ADD_TEST( suite, test_uri_norm_path_3);
	SUITE_ADD_TEST( suite, test_uri_norm_path_4);
	SUITE_ADD_TEST( suite, test_uri_norm_ipv4_1);
	SUITE_ADD_TEST( suite, test_uri_norm_ipv4_2);
	SUITE_ADD_TEST( suite, test_uri_norm_ipv4_3);
	SUITE_ADD_TEST( suite, test_uri_norm_ipv4_4);
	SUITE_ADD_TEST( suite, test_uri_norm_ipv4_5);
	SUITE_ADD_TEST( suite, test_uri_norm_ipv4_6);
	SUITE_ADD_TEST( suite, test_uri_norm_ipv4_7);
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

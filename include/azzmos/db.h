/*
 * =====================================================================================
 *
 *       Filename:  db.h
 *
 *    Description:  Database functions, tailored to PL/SQL
 *
 *        Version:  1.0
 *        Created:  11/10/2010 21:46:33
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Aaron Spiteri
 *        Company:  
 *
 * =====================================================================================
 */

/* #####   HEADER FILE INCLUDES   ################################################### */
#define __AZZMOS_DB_H__
#ifndef __AZZMOS_COMMON_H__
#include <azzmos/common.h>
#endif

/*=====================================================================================
 * To find Oid these you need to look into server/catalog/pg_type.h,  which is
 * a very messy place. 
 *
 * However most (all) OID are the type name in capitals plus OID. So a guess should 
 * be good enough.
 *
 * Do not include in the pg_types file in code,  bad things
 * happen when you try.
 *
 *=====================================================================================
 */
#define INT4OID      23
#define TEXTOID      25
#define VARCHAROID 1043

/* #####   EXPORTED FUNCTION DECLARATIONS   ######################################### */
extern int  db_alloc(PGconn **db, const char *connstr);
extern int  db_init(PGconn *db, const char *cliencoding);
extern void db_free(PGconn *db);

/* #####   EXPORTED MACROS   ######################################################## */
#define DB_ERROR_C(m, c) ERROR_B(m, PQerrorMessage(c))

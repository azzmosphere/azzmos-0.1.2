/*
 * =====================================================================================
 *
 *       Filename:  db.c
 *
 *    Description:  Database functions, tailored to PL/SQL
 *
 *        Version:  1.0
 *        Created:  11/10/2010 22:10:33
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Aaron Spiteri
 *        Company:  
 *
 * =====================================================================================
 */

#include <azzmos/db.h>

#define CLIENT_ENC "client_encoding"
#define SQL_CLIENC "SET client_encoding = %1;"
/* #####   EXPORTED FUNCTION ######################################################### */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  db_alloc
 *  Description:  Create the DB object.  ERROR(m,n) can be used to print
 *                errors on returned errors here except EAUTH,  if that is returned then
 *                the DB_ERROR_C(m, c) should be used instead.
 * =====================================================================================
 */
extern int  
db_alloc(PGconn **db, const char *connstr)
{
    int result = 0;
    PGconn *d;
    if( !connstr){
        return EINVAL;
    }
    d = PQconnectdb( connstr );
    if( PQstatus(d) == CONNECTION_BAD){
        result = EAUTH;
    }
    else {
        *(db) = d;
    }
    return result;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  db_init
 *  Description:  Perform any SQL functions required to initilize the database.
 * =====================================================================================
 */
extern int  
db_init(PGconn *db, const char *cliencoding)
{
    int result = 0;
    const Oid paramTypes[1] = {TEXTOID};
    const char *const paramValues[1] = {cliencoding};
    char *enc = (char *) PQparameterStatus(db, CLIENT_ENC);
    PGresult *hdl;
    
    if(strncmp(cliencoding, enc, BUFSIZ) != 0 ){
        hdl = PQexecParams(db,SQL_CLIENC,1,paramTypes,paramValues,NULL,NULL,0);
        if(!hdl){
            result = EINVAL;
        }
        else if(PQresultStatus( hdl ) != PGRES_COMMAND_OK) {
            result = PQresultStatus( hdl );
        }
    }
    return result;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  db_free
 *  Description:  Free the database connection.
 * =====================================================================================
 */
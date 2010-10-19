/*
 * =====================================================================================
 *
 *       Filename:  httpheaders.h
 *
 *    Description:  HTTP header types and related functions
 *        Version:  1.0
 *        Created:  07/10/2010 22:58:34
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Aaron Spiteri
 *        Company:  
 *
 * =====================================================================================
 */

/* #####   HEADER FILE INCLUDES   ################################################### */
#define _AZZMOS_HTTPHEADERS_H_
#ifndef __AZZMOS_COMMON_H__
#include <azzmos/common.h>
#endif
#ifndef __AZZMOS_URIOBJ_H__
#include <azzmos/uriobj.h>
#endif

/* #####   TYPE DEFINITIONS   ####################################################### */

struct httpheaders_s {
    char            *he_key;   /* The name of the headers */
    char            *he_value; /* The header value*/
    struct list_head he_list;  /* linked list to next header */
} typedef httpheaders_t;

/* #####   FUNCTION DEFINITIONS   ################################################## */
extern       httpheaders_t *httpheaders_alloc();
extern void  httpheaders_free(httpheaders_t *hd);
extern int   httpheaders_init(httpheaders_t **hd);
extern int   httpheaders_build(httpheaders_t **hd, const char *ptr);
extern char *httpheader_value(const httpheaders_t *hd, const char *key);

/*
 * =====================================================================================
 *
 *       Filename:  uriresolve.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  16/09/2010 22:18:11
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Aaron Spiteri
 *        Company:  
 *
 * =====================================================================================
 */


/* #####   HEADER FILE INCLUDES   ################################################### */
#define __AZZMOS_URIRESOLVE_H__
#ifndef __AZZMOS_COMMON_H__
#include <azzmos/common.h>
#endif
#ifndef __AZZMOS_URIOBJ_H__
#include <azzmos/uriobj.h>
#endif
#ifndef _AZZMOS_REGEXPR_H_
#include <azzmos/regexpr.h>
#endif
#ifndef __AZZMOS_URIPARSE_H__
#include <azzmos/uriparse.h>
#endif

/* #####   EXPORTED FUNCTION DECLARATIONS   ######################################### */
/**************************************************************************************
 * Currently this implementation uses PF_INET instead of PF_UNSPEC,  this is because 
 * the resolution of IPv6 still unreliable on the internet at the moment.  However it
 * is expected that this will change one day and when it does PF_UNSPEC should be 
 * used.
 **************************************************************************************
 */
#define URI_AI_FAMILY PF_INET

/* #####   EXPORTED FUNCTION DECLARATIONS   ######################################### */
extern int uri_resolve( uriobj_t **uri);
extern int ref_resolve(uriobj_t **uri, const uriobj_t *base,const char *href, regexpr_t *re, const bool strict);
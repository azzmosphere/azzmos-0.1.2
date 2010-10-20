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

/* #####   EXPORTED FUNCTION DECLARATIONS   ######################################### */
extern int uri_resolve( uriobj_t **uri);
uriobj_t * ref_resolve(char *href, const uriobj_t *base, regexpr_t *re, const bool strict);
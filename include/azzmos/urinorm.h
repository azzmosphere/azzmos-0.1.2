/*
 * =====================================================================================
 *
 *       Filename:  urinorm.c
 *
 *    Description: Parsing function for URI objects..
 *
 *        Version:  1.0
 *        Created:  22/09/2010 22:15:33
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Aaron Spiteri
 *        Company:  
 *
 * =====================================================================================
 */

/* #####   HEADER FILE INCLUDES   ################################################### */
#define __AZZMOS_URINORM_H__
#ifndef __AZZMOS_COMMON_H__
#include <azzmos/common.h>
#endif
#ifndef _AZZMOS_UTILS_H_
#include <azzmos/utils.h>
#endif
#ifndef __AZZMOS_URIOBJ_H__
#include <azzmos/uriobj.h>
#endif

/* #####   EXPORTED FUNCTION DECLARATIONS   ######################################### */
extern int uri_norm_scheme( char **scheme_in);
extern int uri_norm_reg_name( char **regname);
extern int uri_norm_port( char **pin);
extern int uri_norm_path( char **pathin);
/*
 * =====================================================================================
 *
 *       Filename:  uriparse.c
 *
 *    Description: Parsing function for URI objects..
 *
 *        Version:  1.0
 *        Created:  20/09/2010 22:15:33
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Aaron Spiteri
 *        Company:  
 *
 * =====================================================================================
 */

/* #####   HEADER FILE INCLUDES   ################################################### */
#define __AZZMOS_URIPARSE_H__
#ifndef __AZZMOS_COMMON_H__
#include <azzmos/common.h>
#endif
#ifndef _AZZMOS_UTILS_H_
#include <azzmos/utils.h>
#endif
#ifndef __AZZMOS_URIOBJ_H__
#include <azzmos/uriobj.h>
#endif
#ifndef _AZZMOS_REGEXPR_H_
#include <azzmos/regexpr.h>
#endif

/* #####   EXPORTED FUNCTION DECLARATIONS   ######################################### */
extern char *uri_remove_dot_segments( const char *path );
extern char *pop_segment( char **path);
extern char *shift_segment( char **path, const int offset);
extern char *replace_prefix( char **path);
extern char *get_next_segment( char **path);
extern int   uri_init_regex( regexpr_t *re);
extern int   uri_parse( uriobj_t **uri, regexpr_t *re, const char *fqp);
extern char *uri_merge_paths(  const uriobj_t *rel, const uriobj_t *base);
extern char *uri_comp_recomp( const uriobj_t *uri);
extern uriobj_t *uri_trans_ref(const uriobj_t *refin, const uriobj_t *base, const bool strict);
extern int       uri_parse_auth(uriobj_t **uri);
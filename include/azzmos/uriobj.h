/*
 * =====================================================================================
 *
 *       Filename:  uriobj.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  19/09/2010 13:46:33
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Aaron Spiteri
 *        Company:  
 *
 * =====================================================================================
 */


/* #####   HEADER FILE INCLUDES   ################################################### */
#define __AZZMOS_URIOBJ_H__
#ifndef __AZZMOS_COMMON_H__
#include <azzmos/common.h>
#endif
#ifndef _AZZMOS_UTILS_H_
#include <azzmos/utils.h>
#endif

/* #####   EXPORTED MACROS   ######################################################## */

/* #####   EXPORTED TYPE DEFINITIONS   ############################################## */
#define URI_REGNAME    0x01
#define URI_IPV6       0x02
#define URI_INVALID    0x04
#define URI_IP         0x08
#define URI_PORT       0x10

/* #####   EXPORTED DATA TYPES   #################################################### */
struct uriobj_s {
	long   uri_id;              /* unique identifier for URI */
	char *uri_scheme;          /* The scheme section */
	char *uri_auth;            /* authority section */
	char *uri_path;            /* path section  */
	char *uri_query;           /* query section */
	char *uri_frag;            /* fragment section */
	char *uri_host;            /* hostname of URI */
	char *uri_port;            /* uri port number */
	long   uri_flags;           /* various flags for the uri */
	struct addrinfo *uri_ipv4; /* list of the URI resolved addresses */
	struct addrinfo *uri_ipv6; /* list of the URI resolved addresses */
} typedef uriobj_t;

/* #####   EXPORTED FUNCTION DECLARATIONS   ######################################### */
extern uriobj_t *uri_alloc();
extern void      uri_free( uriobj_t *uri);
extern int       uri_clone(uriobj_t **ref, const uriobj_t *base);

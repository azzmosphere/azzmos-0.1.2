/*
 * =====================================================================================
 *
 *       Filename:  uriobj.c
 *
 *    Description:  Implements RFC3986 using the uriobj_t object to define
 *                  a URI.
 *
 *        Version:  1.0
 *        Created:  19/09/2010 22:28:38
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Aaron Spiteri
 *        Company:  
 *
 * =====================================================================================
 */

/* #####   HEADER FILE INCLUDES   ################################################### */
#include <azzmos/uriobj.h>

/* #####   FUNCTION DEFINITIONS  -  EXPORTED FUNCTIONS   ############################ */
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  uri_alloc
 *  Description:  Create and initlize a new URI object.  All pointers are initilized 
 *                to NULL and the flags and id are set to '0'
 * =====================================================================================
 */
 extern uriobj_t *
 uri_alloc()
 {
	uriobj_t *uri = (uriobj_t *) calloc(sizeof(uriobj_t),2);
	if( uri ) {
		uri->uri_id = uri->uri_flags = 0;
		uri->uri_scheme = uri->uri_auth  =
		                  uri->uri_path  =
						  uri->uri_query =
						  uri->uri_frag  =
						  uri->uri_host  =
						  uri->uri_port  =
						  NULL;
		uri->uri_ip  =  NULL;
	}
	return uri;
 }

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  uri_free
 *  Description:  free all members of the URI object and then free the URI object itself.
 * =====================================================================================
 */
extern void     
uri_free( uriobj_t *uri)
{
	struct addrinfo *addr;
	if( uri ) {
		safe_free(uri->uri_scheme);
		safe_free(uri->uri_auth);
		safe_free(uri->uri_path);
		safe_free(uri->uri_query);
		safe_free(uri->uri_frag);
		safe_free(uri->uri_host);
		safe_free(uri->uri_port);
		
		/* After a clone we do not want to free address info */
		if(uri->uri_ip){
			addr = uri->uri_ip;
			freeaddrinfo(addr);
		}
		free(uri);
	}
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  uri_clone
 *  Description:  clone the URI object base to ref, except address info.  For that we 
 *                keep the address.
 * =====================================================================================
 */
 extern int       
 uri_clone(uriobj_t **uri, const uriobj_t *base)
 {
	uriobj_t *u = uri_alloc();
	if( errno ) {
		return errno;
	}
	u->uri_flags = base->uri_flags;
	u->uri_id    = base->uri_id;
	u->uri_scheme= URI_CP_PT(base->uri_scheme);
	u->uri_auth  = URI_CP_PT(base->uri_auth);
	u->uri_path  = URI_CP_PT(base->uri_path);
	u->uri_frag  = URI_CP_PT(base->uri_frag);
	u->uri_host  = URI_CP_PT(base->uri_host);
	u->uri_port  = URI_CP_PT(base->uri_port);
	u->uri_query = URI_CP_PT(base->uri_query);
	*uri = u;
	return errno;
 }
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
 *         Name:  free_uriip
 *  Description:  free the uriip structure.
 * =====================================================================================
 */
static void
free_uri_ip(uri_ip_t *ip)
{
    safe_free(ip->ip_addr);
    safe_free(ip->ip_cname);
    free(ip);
    ip = NULL;
}

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
     uriobj_t *uri = (uriobj_t *) calloc(1,sizeof(uriobj_t));
     if( uri ) {
         uri->uri_id = uri->uri_flags = 0;
         uri->uri_scheme = uri->uri_auth  =
            uri->uri_path  =
            uri->uri_query =
            uri->uri_frag  =
            uri->uri_host  =
            uri->uri_port  = NULL;
         uri->uri_ip   =  NULL;
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
    uri_ip_t *tmp, *ip;
    struct list_head *pos, *n;
    if( uri ) {
        safe_free(uri->uri_scheme);
        safe_free(uri->uri_auth);
        safe_free(uri->uri_path);
        safe_free(uri->uri_query);
        safe_free(uri->uri_frag);
        safe_free(uri->uri_host);
        safe_free(uri->uri_port);
        
        /* free the ip address info */
        if(uri->uri_ip){
            ip = uri->uri_ip;
            list_for_each_safe(pos, n, &ip->ip_list){
                tmp = list_entry(pos, uri_ip_t, ip_list);
                list_del(&tmp->ip_list);
                free_uri_ip(tmp);
            }
            free_uri_ip(ip);
        }
        free(uri);
        uri = NULL;
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
     int err = 0;
     uriobj_t *u = uri_alloc();
     if( !u ) {
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
     *(uri) = u;
     return err;
 }

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  uri_free_list
 *  Description:  Free the entire linked list of URI objects
 * =====================================================================================
 */
extern void      
uri_free_list(uriobj_t *list)
{
    uriobj_t *uri;
    struct list_head *pos, *n;
    list_for_each_safe(pos, n, &list->uri_list){
        uri = list_entry(pos, uriobj_t, uri_list);
        list_del(&uri->uri_list);
        uri_free(uri);
    }
    uri_free(list);
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  uri_alloc_list
 *  Description:  Allocate memory and initilize the URI list object
 * =====================================================================================
 */
extern uriobj_t *
uri_alloc_list()
{
    uriobj_t *list = uri_alloc();
    if(list){
        INIT_LIST_HEAD(&list->uri_list);
    }
    return list;
}
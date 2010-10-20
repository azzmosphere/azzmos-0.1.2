/*
 * =====================================================================================
 *
 *       Filename:  uriresolve.c
 *
 *    Description: URI resolition functions, these functions resolve a URI to its
 *                 full path when given a base.  They are used as part of the 
 *                 downloader
 *
 *        Version:  1.0
 *        Created:  27/09/2010 22:15:33
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Aaron Spiteri
 *        Company:  
 *
 * =====================================================================================
 */

/* #####   HEADER FILE INCLUDES   ################################################### */
#include <azzmos/uriresolve.h>
/* #####   FUNCTION DEFINITIONS  -  LCOAL FUNCTIONS   ############################### */
static void
return_err(const char *key, const char *val, const char *msg, int err)
{
	DEBUG_STR(key, val);
	ERROR_E(msg, err);
}

/* #####   FUNCTION DEFINITIONS  -  EXPORTED FUNCTIONS   ############################ */
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  create_ip_list
 *  Description:  Internal function used to create the URI address linked list.
 * =====================================================================================
 */
static int 
create_ip_list(uri_ip_t **ipin, const struct addrinfo *addr)
{
    int rc = 0;
    uri_ip_t *ipfirst = NULL, 
             *ip = NULL;
    struct sockaddr_in *addr_in;
    while( addr != NULL){
        ip =  calloc(sizeof(uri_ip_t),1);
        if(!ip){
            return EAI_SYSTEM;
        }
        if( !ipfirst){
            INIT_LIST_HEAD(&ip->ip_list);
        }
        addr_in = (struct sockaddr_in *)addr->ai_addr;
        ip->ip_addr = calloc(addr->ai_addrlen,1);
        if(!ip->ip_addr){
            return EAI_SYSTEM;
        }
        strncpy(ip->ip_addr, inet_ntop(addr->ai_family,
                                       &addr_in->sin_addr,
                                       ip->ip_addr,
                                       addr->ai_addrlen),
                (addr->ai_addrlen - 1)
        );
        ip->ip_ai_family = addr->ai_family;
        ip->ip_cname     = strdup(addr->ai_canonname);
        addr = addr->ai_next;
        if(!ipfirst){
            ipfirst = ip;
        }
        else {
            list_add(&ip->ip_list, &ipfirst->ip_list);
        }
    } 
    *(ipin) = ipfirst;
    return rc; 
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  uri_resolve
 *  Description:  Use the DNS server or underlying OS resolver to fill out URI attributes.
 *                On error the gai_error code is returned, otherwise the return value is 
 *                '0',  if the error is a standard error then EAI_SYSTEM is returned and
 *                errno is set.
 * =====================================================================================
 */
extern int 
uri_resolve( uriobj_t **uri)
{
	int gai_error = 0;
	struct addrinfo hints,
	               *addr,
                   *addrfirst;
	uriobj_t *trans = *(uri);
    uri_ip_t *ip;
	
	/* create a tranisient URI to leave uri untouched on error*/
	if( gai_error ) {
		errno = gai_error;
		return EAI_SYSTEM;
	}
	bzero(&hints, sizeof(hints));
    bzero(&addr, sizeof(addr));
	
	/* For this application assume TCP sockets */
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_family   = PF_UNSPEC;
	
	/* calculate what needs to be returned */
	if( trans->uri_flags & URI_IP){
		hints.ai_flags =  AI_NUMERICHOST;
	}
	/* Make DNS call */
	hints.ai_flags |= AI_CANONNAME|AI_PASSIVE;
	gai_error = getaddrinfo(trans->uri_host, trans->uri_scheme, &hints, &addr);
	
	/* all things going well copy the address structure to uri */
	if( ! gai_error ){
        gai_error = create_ip_list(&trans->uri_ip, addr);
        
        /* free up memory and copy the newly created reference to uri */
        if(addr){
            freeaddrinfo(addr);
            addr = NULL;
        }
	}
	return gai_error;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  ref_resolve
 *  Description:  This function is used by the downloader to normalize and resolve a
 *                href in a given HTML page to a URI object.  The base should be the
 *                URI object that contains the HREF and strict refers to RFC3986 section
 *                5.2.2 which describes the behavour the algoritm should take if the 
 *                scheme section of the URI is absent.
 *
 *                The calling routine of this function should check the return objects
 *                uri_flags for URI_INVALID, this should not be a critical error rather
 *                it should instruct qu_ok_download not to download the URI.
 *                
 *                It should also check to see if the return value is NULL, if it is then
 *                a error should be raised printing errno string reference as this will
 *                be a memory allocation problem.
 * =====================================================================================
 */
uriobj_t *
ref_resolve(char *href, const uriobj_t *base, regexpr_t *re, const bool strict)
{
	uriobj_t *trans,
			 *ref = uri_alloc();
	if( errno ){
		ERROR("could not allocate memory for reference object");
		return NULL;
	}
	int err = uri_parse(&ref,re,href);
	if(err){
		ref->uri_flags |= URI_INVALID;
		return_err("href",href,"could not parse href to uriobj", err);
		return ref;
	}
	if(ref->uri_path){
		err = uri_norm_path(&ref->uri_path);
		if( err ){
			ref->uri_flags |= URI_INVALID;
			return_err("path",ref->uri_path,"could not parse path", err);
			return ref;
		}
	}
	trans = uri_trans_ref(ref,base,strict);
	if( !trans){
		ERROR("could not create target uri");
		ref->uri_flags |= URI_INVALID;
		return ref;
	}
	err = uri_parse_auth(&trans);
	if( !err){
		err = uri_norm_scheme(&trans->uri_scheme);
	}
	if( !err){
		if( trans->uri_flags & URI_IP){
			if(trans->uri_flags & URI_IPV6){
				err = uri_norm_ipv6(&trans->uri_host);
			}
			else {
				err = uri_norm_ipv4(&trans->uri_host);
			}
		}
		else if(trans->uri_flags & URI_REGNAME){
			err = uri_norm_reg_name(&trans->uri_host);
		}
	}
	if( !err){
		err = uri_norm_port(&trans->uri_port);
	}
	else {
		ERROR_B("could not normalize uri",err);
		ref->uri_flags |= URI_INVALID;
	}
	if(!err){
		err = uri_resolve(&trans);
		if( err ){
			if( err == EAI_SYSTEM){
				ERROR("could not resolve uri");
			}
			else{
				ERROR_B("could not resolve error", gai_strerror(err));
			}
		}
		else{
			uri_free(ref);
			ref = uri_alloc();
			uri_clone(&ref,trans);
		}
	}
	return ref;
}
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
#include <azzmos/uriparse.h>

/* #####   MACROS  -  LOCAL TO THIS SOURCE FILE   ################################### */
#define RE_ID 0
#define RE    "^(([^:/?#]+):)?(//([^/?#]*))?([^?#]*)(\\?([^#]*))?(#(.*))?"

/**************************************************************************************
 * PCRE has the following equivilants for PERL subqueries,  that is $1 = ovector[2] and
 * ovector[3]. Below is a description of how these map to RFC3986 Appendix B regular 
 * expression which is defined by the macro RE.
 *    $0 = 0,1
 *    $1 = 2,3
 *    $2 = 4,5
 *    $3 = 6,7
 *    $4 = 8,9
 *    $5 = 10,11
 *    $6 = 12,13
 *    $7 = 14,15
 *    $8 = 16,17
 *    $9 = 18,19
 **************************************************************************************/
#define RE_S_S 0x04
#define RE_S_E 0x05
#define RE_A_S 0x08
#define RE_A_E 0x09
#define RE_P_S 0x0a
#define RE_P_E 0x0b
#define RE_Q_S 0x0e 
#define RE_Q_E 0x0f
#define RE_F_S 0x12
#define RE_F_E 0x13

/* #####   FUNCTION DEFINITIONS  -  INTERNAL FUNCTIONS   ############################# */
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  uri_strcat
 *  Description:  Concadinate s2 to s1 using a format string.  The format string should
 *                contain two '%s'.  This is a wrapper function to stop memory leaks.
 * =====================================================================================
 */
static char *
uri_strcat( char **s1, const char *format, const char *s2)
{
        char *buf;
        asprintf(&buf,format,*(s1),s2);
        *(s1) = NULL;
        free(*s1);
        *s1 = buf;
        return *s1;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  uri_strcpy
 *  Description:  Copy s2 to s1. Free s1 before copying to avoid memory leaks.
 * =====================================================================================
 */
static void
uri_strcpy(char **s1, const char *s2)
{
	char *s = URI_CP_PT(s2);
	free(*s1);
	*s1 = s;
}

/* #####   FUNCTION DEFINITIONS  -  EXPORTED FUNCTIONS   ############################ */
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  uri_remove_dot_segments
 *  Description:  Implements RFC 3986 section 5.2.4 
 *                use for interpreting and removing the special "." and ".." complete 
 *                path segments from a referenced path.  This is done after the path is
 *                extracted from a reference, whether or not the path was relative, in
 *                order to remove any invalid or extraneous dot-segments prior to
 *                forming the target URI.  Although there are many ways to accomplish
 *                this removal process, we describe a simple method using two string
 *                buffers.
 *                
 * =====================================================================================
 */
extern char *
uri_remove_dot_segments( const char *path )
{
        char *ou_buffer = NULL,
             *in_buffer = NULL,
             *segment   = NULL,
             sl = '\0';
        int len = 0;
        if( path == NULL ) {
                return NULL;
        }
        in_buffer = strdup( path);
        len = strlen( in_buffer ) + 1;
        ou_buffer = (char *) malloc( len );
        ou_buffer[0] = '\0';
        while( in_buffer != NULL ){
                segment = get_next_segment( &in_buffer);
                if( strcmp( segment, "../") == 0 || strcmp(segment,"./") == 0){
                        shift_segment( &in_buffer, 0);
                }
                else if( strcmp(segment, "/./") == 0 || strcmp(segment,"/.") == 0){
                        replace_prefix( &in_buffer);
                }
                else if( strcmp( segment,"/../") == 0 || strcmp(segment,"/..") == 0){
                        replace_prefix(&in_buffer);
                        pop_segment(&ou_buffer);
                }
                else if( strcmp( in_buffer, ".") == 0 || strcmp( in_buffer, "..") == 0){
                        in_buffer = NULL;
                }
                else if( segment) {
                        sl = segment[ strlen(segment) - 1];
                        if( sl == '/'){
                                segment = shift_segment(&in_buffer, 0);
                        }
                        else {
                                segment = shift_segment(&in_buffer, 1);
                        }
                        strcat(ou_buffer, segment);
                }
        }
        len = strlen(ou_buffer) + 1;
        realloc( (void *) ou_buffer, len);
		return ou_buffer;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  pop_segment
 *  Description: get the last segment in the path not including
 *               the initial "/" character (if any) and any subsequent characters up to, 
 *               but an including, the last "/"
 *
 * =====================================================================================
 */
extern char *
pop_segment( char **path)
{
        int i = 0,
            s = 0,
            len = 0;
        char *segment,
             *pref = URI_CP_PT(*path);
        if( !pref || !strlen(pref)){
                return NULL;
        }
        len = i = strlen(pref);
        for(; i > 0; i --){
                if( pref[i] == '/'){
                        break;
                }

        }
        *(path) = usplice(pref, 0, (i -1));
        segment = (char *) malloc( len + 1);
        for(; i < len; i ++){
                segment[s ++] = pref[i];
        }
        segment[s] = '\0';
        realloc( (void *) segment, s);
        return segment;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  shift_segment
 *  Description: get the first path segment from the path, including the initial 
 *               "/" character (if any) and any subsequent characters up to, 
 *               but not including, the next "/", reset
 *
 * =====================================================================================
 */
extern char *
shift_segment( char **path, const int offset)
{
        int  len = 0,
             i = 0;
        char *segment,
             *pref = URI_CP_PT(*path);
        if( ! pref  || ! strlen(pref) ){
                return NULL;
        }
        len = strlen( pref );
        segment = (char *) malloc( len + 1);
        segment[0] = pref[0];
        for( i = 1; i < len; i ++){
                if( pref[i] == '/' ) {
                        break;
                }
                segment[i] = pref[i];
        }
        *(path) = usplice( pref, (i + offset), (unsigned int) NULL);
        segment[++i] = '\0';
        realloc( (void *) segment, i);
        return segment;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  replace_prefix
 *  Description:  replace the prefix segment of path with '/'
 * =====================================================================================
 */
extern char *
replace_prefix( char **path)
{
        char  *pref,
              *segment,
              *tmp;
        if( *(path) == NULL || !strlen(*(path))){
                return NULL;
        }
        pref = strdup( *(path));
        segment = shift_segment( &pref, 1);
        if( pref == NULL){
                tmp = strdup("/");
        }
        else{
                tmp = (char *) malloc( strlen(pref) + 2);
                tmp[0] = '/';
                tmp[1] = '\0';
                strcat(tmp, pref);
        }
        *(path) = tmp;
        return segment;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  get_next_segment
 *  Description:  return the next segment from the path
 * =====================================================================================
 */
extern char *
get_next_segment( char **path)
{
        char *pref,
             *segment;
        int i = 0,
            len = 0;
        if( *(path) == NULL || !strlen(*(path))){
                return NULL;
        }
        pref = strdup( *(path));
        len  = strlen( pref );
        segment = (char *) malloc( strlen(pref));
        segment[i] = pref[i];
        if( strlen(pref) > 1 ) {
                for( i = 1; i < len; i ++){
                        segment[i] = pref[i];
                        if( segment[i] == '/'){
                                break;
                        }
                }
        }
        segment[ ++i] = '\0';
        realloc( (void *) segment, i);
        return segment;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  uri_init_regex
 *  Description:  Initilises the regular expresion that is used break up the FQP into a 
 *                uri object.  This function must be the first function called by the
 *                scheduler when it is initlizing the regular expersion object.
 * =====================================================================================
 */
extern int
uri_init_regex( regexpr_t *re)
{
        int err = 0;
        err = re_init(re, RE_ID);
        if( err == 0 ) {
                err = re_comp( re, RE_ID, RE, 0, NULL);
        }
        return err;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  uri_parse
 *  Description:  URI objects can be created in two ways: 
 *                1) By downloading the uri from the database; or
 *                2) By creating the URI using a FQP string.
 *
 *                The uri_parse functin does not assume that the URI exists before it
 *                creates the object.  When it is call the object is created without an
 *                id.  This will need to be allocated by uri_db_update and should be done
 *                after the path is resolved.
 *
 *                On success the uri object will have the members returned by the re 
 *                allocated and zero will be returned.  On failure it will return a URI 
 *                offset error.
 * =====================================================================================
 */
extern int
uri_parse( uriobj_t **uri, regexpr_t *re, const char *fqp)
{
        int err = 0;
		uriobj_t *trans = *uri;
        if( !trans ) {
                return errno;
        }
        *(re->re_subject) = strdup(fqp);
        re->re_length = strlen(fqp);
        err = re_exec( re, RE_ID);
        if( err > 0 ) {
                err = 0;
                trans->uri_scheme = usplice(fqp, re->re_ovector[RE_S_S], re->re_ovector[RE_S_E] -1);
                trans->uri_auth   = usplice(fqp, re->re_ovector[RE_A_S], re->re_ovector[RE_A_E] -1);
                trans->uri_path   = usplice(fqp, re->re_ovector[RE_P_S], re->re_ovector[RE_P_E] -1);
                trans->uri_query  = usplice(fqp, re->re_ovector[RE_Q_S], re->re_ovector[RE_Q_E] -1);
                trans->uri_frag   = usplice(fqp, re->re_ovector[RE_F_S], re->re_ovector[RE_F_E] -1);
                trans->uri_id = trans->uri_flags = 0;
                trans->uri_host = trans->uri_port
								= NULL;
				trans->uri_ipv4 = trans->uri_ipv6 = NULL;
        }
        return err;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  uri_merge_paths
 *  Description:  This a attempt at implementing RFC3986 section 5.2.3. 
 *
 *                If the base URI has a defined authority component and an empty 
 *                path, then return a string consisting of "/" concatenated with the 
 *                reference's path; otherwise, 
 *
 *                return a string consisting of the reference's path component 
 *                appended to all but the last segment of the base URI's path (i.e., 
 *                excluding any characters after the right-most "/" in the base URI 
 *                path, or excluding the entire base URI path if it does not contain 
 *                any "/" characters).
 *  
 * =====================================================================================
 */
extern char *
uri_merge_paths(  const uriobj_t *rel, const uriobj_t *base)
{
        char *path,
             *bpath = URI_CP_PT(base->uri_path),
             *rpath = URI_CP_PT(rel->uri_path);
        int   len = 0;
        if( base->uri_auth && !(bpath) ) {
                if( !(rpath)){
                        path = strdup("/");
                }
                else {
                        len = strlen(rpath) + 2;
                        path = (char *) malloc(len * sizeof(char));
                        path[0] = '/';
                        path[1] = '\0';
                        strncat(path,rpath,len);
                }
        }
        else {
                pop_segment(&bpath);
                len = strlen(rpath) + strlen(bpath) + 1;
                path = (char *) malloc(len * sizeof(char));
                strncat(path, bpath, (len -1));
                strncat(path, strdup("/"), (len - strlen(path)));
                strncat(path, rpath, (len - strlen(path)));
        }
        return path;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  uri_comp_recomp
 *  Description: Parsed URI components can be recomposed to obtain the corresponding 
 *               URI reference string. This is a attempt at implementing RFC3986 section
 *               5.3.
 * =====================================================================================
 */
extern char *
uri_comp_recomp( const uriobj_t *uri)
{
        char *result;
        if( uri->uri_scheme ) {
                asprintf(&result, "%s:",uri->uri_scheme);
        }
        if( uri->uri_auth){
                result = uri_strcat(&result,"%s//%s", uri->uri_auth);
        }
        result = uri_strcat(&result,"%s%s",uri->uri_path);
        if( uri->uri_query ){
                result = uri_strcat(&result,"%s?%s",uri->uri_query);
        }
        if( uri->uri_frag ) {
                result = uri_strcat(&result,"%s#%s",uri->uri_frag);
        }
        return result;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  uri_trans_ref
 *  Description:  Attempt to implement algorithm 5.2.2 of RFC3986, For each URI reference
 *               (R), transform R into its target URI (T). The return value of this 
 *               routine is (T).
 * =====================================================================================
 */
extern uriobj_t *
uri_trans_ref(const uriobj_t *refin, const uriobj_t *base, const bool strict)
{
        uriobj_t *trans = uri_alloc(),
		         *ref   = uri_alloc();
		int e = uri_clone(&ref, refin);
        if( e ) {
                return NULL;
        }
        if( ! strict && (strcmp(UI(base->uri_scheme), UI(ref->uri_scheme)) == 0)){
                ref->uri_scheme = NULL;
        }
        if( ref->uri_scheme){
                trans->uri_scheme = URI_CP_PT(ref->uri_scheme);
                trans->uri_auth   = URI_CP_PT(ref->uri_auth);
                trans->uri_path   = uri_remove_dot_segments( ref->uri_path );
                trans->uri_query  = URI_CP_PT(ref->uri_query);
        }
        else {
                if( ref->uri_auth ){
                        trans->uri_auth = URI_CP_PT(ref->uri_auth);
                        trans->uri_path = uri_remove_dot_segments( ref->uri_path );
                        trans->uri_query = URI_CP_PT(ref->uri_query);
                }
                else{
                        if( ! ref->uri_path ){
                                trans->uri_path   = URI_CP_PT(base->uri_path);
                                if( ref->uri_query){
                                        trans->uri_query = URI_CP_PT(ref->uri_query);
                                }
                                else {
                                        trans->uri_query = URI_CP_PT(base->uri_query);
                                }
                        }
                        else {
                                if( ref->uri_path[0] == '/'){
                                        trans->uri_path = uri_remove_dot_segments(ref->uri_path);
                                }
                                else{
                                        trans->uri_path = uri_merge_paths(ref, base);
                                        uri_strcpy(&trans->uri_path, uri_remove_dot_segments(trans->uri_path));
                                }
                                trans->uri_query = URI_CP_PT(ref->uri_query);
                        }
                        uri_strcpy(&trans->uri_auth, base->uri_auth);
                }
                uri_strcpy(&trans->uri_scheme, base->uri_scheme);
        }
        trans->uri_frag = URI_CP_PT(ref->uri_frag);
		uri_free(ref);
        return trans;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  uri_parse_auth
 *  Description:  Set's the host string and port string,  also will set the URI_IP or
 *                URI_REGNAME and URI_IPV6 flag.  On error the appropriate error code 
 *                will be returned and the uri object will be left untouched.
 * =====================================================================================
 */
extern int
uri_parse_auth(uriobj_t **uri)
{
	int err = 0,
	    n   = 0,
		i   = 0,
		len = 0;
	bool  ipv6 = false;
	char *auth,
	     *port,
		 *host,
		 *buffer;
	uriobj_t *trans = uri_alloc();
	uri_clone(&trans, *uri);
	auth = URI_CP_PT(trans->uri_auth);
	if(!auth){
		return EINVAL;
	}
	if( isalpha(auth[0])){
		trans->uri_flags |= URI_REGNAME;
	}
	else if(auth[0] == '['){
		trans->uri_flags |= URI_IPV6|URI_IP;
		i++;
	}
	else if(isnumber(auth[0])){
		trans->uri_flags |= URI_IP;
	}
	else {
		return EILSEQ;
	}
	len = strlen(auth);
	host = (char *) malloc(len + 1);
	if( errno ){
		return errno;
	}
	buffer = host;
	for(;i < len;i++){
		/*
		 * IPV6 end, set it as valid. RFC2732 mandates
		 * that it is to be rejected otherwise.  
		 */
		if( auth[i] == ']' && trans->uri_flags & URI_IPV6){
			ipv6 = true;
			continue;
		}
		if( auth[i] == ':' &&  ((!(trans->uri_flags & URI_IPV6)) != ipv6) ){
			port = (char *)malloc((len - i) + 1);
			buffer = port;
			n = 0;
			continue;
		}
		buffer[n] = auth[i];
		n ++;
	}
	if( trans->uri_flags & URI_IPV6 && !ipv6){
		return EILSEQ;
	}
	trans->uri_host = URI_CP_PT(host);
	trans->uri_port = URI_CP_PT(port);
	if( port){
		trans->uri_flags |= URI_PORT;
	}
	if( !err){
		uri_free(*uri);
		*uri = trans;
	}
	return err;
}

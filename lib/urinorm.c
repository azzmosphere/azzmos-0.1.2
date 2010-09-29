/*
 * =====================================================================================
 *
 *       Filename:  urinorm.c
 *
 *    Description: Web crawler normalization function of a URI.
 *
 *        Version:  1.0
 *        Created:  24/09/2010 22:15:33
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Aaron Spiteri
 *        Company:  
 *
 * =====================================================================================
 */

/* #####   HEADER FILE INCLUDES   ################################################### */
#include <azzmos/urinorm.h>

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  is_pct_encoded
 *  Description:  Checks to see if a string is percentage encoded.
 * =====================================================================================
 */
static bool
is_pct_encoded( char *s ) {
        if( ! s ) {
                return false;
        }
        int len = strlen(s);
        bool rv = true;
        if( len != 3){
                return false;
        }
        if( s[0] != '%' || !isxdigit(s[1]) || !isxdigit(s[2])){
                rv = false;
        }
        return rv;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  is_unreserved
 *  Description:  A unreserved character can appear in any part of the URI 
 * =====================================================================================
 */
static bool
is_unreserved( char c )
{
        bool rv = false;
        if( isalpha(c) ){
                rv = true;
        }
        else if( isdigit(c)){
                rv = true;
        }
        else {
                switch(c){
                        case('-'):
                                rv = true;
                                break;
                        case('.'):
                                rv = true;
                                break;
                        case('_'):
                                rv = true;
                                break;
                        case('~'):
                                rv = true;
                }
        }
        return rv;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  is_sub_delim
 *  Description:  Checks to see if a character is a sub-delim character.
 * =====================================================================================
 */
static bool
is_sub_delim( char c )
{
        bool rv = false;
        switch( c ){
                case('!'):
                        rv = true;
                        break;
                case('$'):
                        rv = true;
                        break;
                case('&'):
                        rv = true;
                        break;
                case('\''):
                        rv = true;
                        break;
                case('('):
                        rv = true;
                        break;
                case(')'):
                        rv = true;
                        break;
                case('*'):
                        rv = true;
                        break;
                case('+'):
                        rv = true;
                        break;
                case(','):
                        rv = true;
                        break;
                case(';'):
                        rv = true;
                        break;
                case('='):
                        rv = true;
        }
        return rv;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  is_gen_delim
 *  Description:  Tests a char to see if it a gen-delim character, returns true for
 *                success or false on failure.
 * =====================================================================================
 */
static bool
is_gen_delims( char c)
{
        bool rv = false;
        switch( c ) {
                case(':'):
                        rv = true;
                        break;
                case('/'):
                        rv = true;
                        break;
                case('?'):
                        rv = true;
                        break;
                case('#'):
                        rv = true;
                        break;
                case('['):
                        rv = true;
                        break;
                case(']'):
                        rv = true;
                        break;
                case('@'):
                        rv = true;
        }
        return rv;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  is_reserved
 *  Description:  Reserved characters can only be used in certain parts of the URI
 *                string.
 * =====================================================================================
 */
static bool
is_reserved( char c )
{
        bool rv = false;
        if( is_gen_delim(c) || is_sub_delim(c) ) {
                rv = true;
        }
        return rv;
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  is_scheme_char
 *  Description:  Check is the character forms a valid URI scheme character.
 * =====================================================================================
 */
static bool
is_scheme_char( char c )
{
        bool rv = false;
        if( isalpha(c) ) {
                rv = true;
        }
        else if( isdigit(c)){
                rv = true;
        }
        else {
                switch(c){
                        case '+':
                                rv = true;
                                break;
                        case '-':
                                rv = true;
                                break;
                        case '.':
                                rv = true;
                }
        }
        return rv;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  is_pchar
 *  Description:  Test if a character is a pchar as desribed by RFC3986.  A pchar
 *                is: unreserved / pct-encoded / sub-delims / ":" / "@", if character
 *                is a pchar return true otherwise return false.
 * =====================================================================================
 */
static bool
is_pchar( char c)
{
        bool result  = false;
        if( is_unreserved(c) || is_sub_delim(c) ){
                result = true;
        }
        else if( c == ':' || c == '@'){
                result = true;
        }
        return result;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  norm_pct
 *  Description:  Normailize a pct-encoded character in accordance to section 2.1 of RFC
 *                3986. 
 * =====================================================================================
 */
static int
norm_pct( char **pct)
{
        char *p = URI_CP_PT(*pct);
        int err = 0;
        if(!is_pct_encoded(p)){
                err = EILSEQ;
        }
        if( err == 0 ) {
                if( isalpha(p[1]) && islower(p[1])){
                        p[1] = toupper(p[1]);
                }
                if( isalpha(p[2]) && islower(p[2])){
                        p[2] = toupper(p[2]);
                }
        }
        *(pct) = NULL;
        free(*pct);
        *(pct) = strdup(p);
        return err;
}

/* #####   FUNCTION DEFINITIONS  -  EXPORTED FUNCTIONS   ############################ */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  uri_norm_scheme
 *  Description:  Normalize the scheme section of the uri. Return 0 on success or 
 *                EILSEQ or ENOATTR 
 * =====================================================================================
 */
extern int
uri_norm_scheme( char **scheme_in)
{
        int rv = 0,
            len,
            i;
        char *scheme = URI_CP_PT(*scheme_in);        
		if( ! scheme || !strlen(scheme)) {
                rv = ENOATTR;
        }
        else if( ! isalpha(scheme[0])){
                rv = EILSEQ;
        }
        else {
                len    = strlen(scheme);
                for(i = 0; i < len;i ++) {
                        if( ! is_scheme_char(scheme[i])){
                                rv = EILSEQ;
                                break;
                        }
                        if( isalpha(scheme[i]) && isupper(scheme[i])){
                                scheme[i] = tolower(scheme[i]);
                        }
                }
        }
        if( rv == 0 ){
                *scheme_in = NULL;
                free(*scheme_in);
                *scheme_in = scheme;
        }
        return rv;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  uri_norm_reg_name
 *  Description:  Normalize the host section of the URI if it is set.  The host section
 *                that is normalized here is the reg-name, and not the IP address.
 *                The IP address should be inside the IP section of the structure.
 *
 *                This section attempts to implement section 3.2.2 of RFC3986 with the
 *                exception that userinfo is not checked as HTTP and HTTPS do not 
 *                specify the usage of it.
 * =====================================================================================
 */
extern int
uri_norm_reg_name( char **regname)
{
        int   err  = 0, i, len;
        char *pct  = (char *) malloc(4),
             *host = URI_CP_PT(*regname);
        if( host ) {
                len = strlen(host);
                for(i = 0;i < len; i ++ ) {
                        if( host[i] == '%'){
                                if( (i + 2) > len ) {
                                        err = EILSEQ;
                                        break;
                                }
                                pct[0] = host[i ++];
                                pct[1] = host[i ++];
                                pct[2] = host[i ++];
                                pct[3] = '\0';
                                err = norm_pct(&pct);
                                if( err ) {
                                        break;
                                }
                                host[(i - 3)] = pct[0];
                                host[(i - 2)] = pct[1];
                                host[(i - 1)] = pct[2];
                                continue;
                        }
                        if(!(is_sub_delim(host[i]) || is_unreserved(host[i]))){
                                err = EILSEQ;
                                break;
                        }
                        if( isalpha(host[i]) && isupper(host[i]) ){
                                host[i] = tolower(host[i]);
                        }
                }
                if( ! err ) {
                        *(regname) = NULL;
                        free(*regname);
                        *(regname) = host;
                }
        }
        return err;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  uri_norm_port
 *  Description:  Normalize the port section.  Just check to see if it a number. This 
 *                should only be set if it is different from the scheme, for example http 
 *                on 8080
 * =====================================================================================
 */
extern int
uri_norm_port( char **pin)
{
        char *port = URI_CP_PT(*pin);
        int err = 0,i, len;
        if( port ) {
                len = strlen(port);
                for(i = 0; i < len; i ++){
                        if( ! isnumber(port[i])){
                                err = EILSEQ;
                        }
                }
        }
        return err;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  uri_norm_path
 *  Description:  Normalize the path,  with this section just check for illegal 
 *                characters.  upper and lower case letters are aloud.
 *
 *                NULL paths are aloud after the URI has been parsed so check that 
 *                the path is not NULL before calling this routine.
 * =====================================================================================
 */
extern int
uri_norm_path( char **pathin)
{
        int err = 0,
            len = 0,
            i   = 0,
            n   = 0;
        char *path = URI_CP_PT(*pathin),
             *ou,
             *pct;
        if( !path){
                return EINVAL;
        }
        len = strlen(path);
        ou  = (char *) malloc(len + 1);
        pct = (char *) malloc(4);
        if( errno ){
                return errno;
        }
        for(;i < len; i ++){
                if( is_pchar(path[i]) || path[i] == '/'){
                        ou[n ++] = path[i];
                }
                else if(path[i] == '%'){
                        if( (i + 2) > len){
                                err = EILSEQ;
                                break;
                        }
                        pct[0] = path[i ++];
                        pct[1] = path[i ++];
                        pct[2] = path[i ++];
                        pct[3] = '\0';
                        err = norm_pct(&pct);
                        if( err ){
                                break;
                        }
                        ou[n ++] = pct[0];
                        ou[n ++] = pct[1];
                        ou[n ++] = pct[2];
                }
                else {
                        err = EILSEQ;
                        break;
                }
        }
        ou[n] = '\0';
        len = strlen(ou) + 1;
        ou = realloc(ou, len);
        free(*pathin);
        *pathin = ou;
        return err;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  uri_norm_ipv4
 *  Description:  Normalise the uri_ip section if it is IPv4.  This should only be done
 *                if the uri_flags does not have URI_IPV6 and URI_IP is set. 
 *                Normalisation for this is (attempted) to be compliant with section 
 *                3.2.2 of RFC 3986.
 *
 *                if a error is returned by this function then the flag URI_IPINVALID 
 *                should be set.
 *
 *                In this function arguing NULL is considered a EINVAL as it assumes 
 *                the above rules.
 *
 *                IPv4 addresses will be determined by RFC1123 section 2.1, EI
 *                #.#.#.#
 *                  dec-octet   = DIGIT                 ; 0-9
 *                              / %x31-39 DIGIT         ; 10-99
 *                              / "1" 2DIGIT            ; 100-199
 *                              / "2" %x30-34 DIGIT     ; 200-249
 *                              / "25" %x30-35          ; 250-255
 *                
 * =====================================================================================
 */
extern int
uri_norm_ipv4( char **ipin)
{
        int err = 0,
            len,
            i,
            offset = 0,
            val,
            segcount = 0;
        char *ip = URI_CP_PT(*ipin),
             *segment = (char *) malloc(4);
        if( ! ip ) {
                err = EINVAL;
        }
        else {
                len = strlen(ip);
                for(i = 0;i < len; i ++){
                        if( ip[i] == '.'){
                                segment[offset] = '\0';
                                offset = 0;
                                segcount ++;
                                if( ! strlen(segment) ){
                                        err = EILSEQ;
                                        break;
                                }
                                if( segcount > 4){
                                        err = ERANGE;
                                        break;
                                }
                                val = atoi(segment);
                                switch(strlen(segment)){
                                        case(1):
                                                break;
                                        case(2):
                                                if(val < 10){
                                                        err = EILSEQ;
                                                }
                                                break;
                                        case(3):
                                                if(val < 100 || val > 255){
                                                        err = EILSEQ;
                                                }
                                                break;
										/* a segment greater than three characters is a
										 * error.
										 */
                                        default:
                                                err = EILSEQ;
                                }
                                if( err ) {
                                        break;
                                }
                                segment = strcpy(segment,"");
                                continue;
                        }
                        else {
                                if( offset > 3){
                                        err = ERANGE;
                                        break;
                                }
                                if( !isnumber(ip[i])){
                                        err = EILSEQ;
                                        break;
                                }
                                segment[offset] = ip[i];
                                offset ++;
                        }
                }
			   if( ! err && segcount != 3 ){
					err = ERANGE;
			   }
        }
        return err;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  uri_norm_ipv6
 *  Description:  Currently this function returns ENOSYS,  as of yet IPv6 support is not
 *                implemented in the downloader.  This should cause the URI_INVALID to 
 *                be set.
 * =====================================================================================
 */
 /* REMAINS TO BE IMPLEMENTED */
extern int
uri_norm_ipv6( char **ip)
{
        return ENOSYS;
}

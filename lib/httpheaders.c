/*
 * =====================================================================================
 *
 * Filename: httpheaders.c
 *
 * Description: HTTP header types and related functions
 * Version: 1.0
 * Created: 07/10/2010 22:58:34
 * Revision: none
 * Compiler: gcc
 *
 * Author: Aaron Spiteri
 * Company:
 *
 * =====================================================================================
 */

#include <azzmos/httpheaders.h>
/*
 * =====================================================================================
 *  httpheaders_alloc
 *
 *  Allocate memory for the HTTP headers object.  After doing this check that it has
 *  not returned a NULL object.  This indicates a ENOMEM error.
 * =====================================================================================
 */
extern       
httpheaders_t *httpheaders_alloc()
{
    httpheaders_t *hd = calloc(sizeof(httpheaders_t),1);
    return hd;
}

/*
 * =====================================================================================
 *  httpheaders_init
 *
 *  initlize the default values of the httheaders_t object
 * =====================================================================================
 */
extern int   
httpheaders_init(httpheaders_t **hd)
{
    int result = 0;
    httpheaders_t *hr = *(hd);
    hr->he_key = hr->he_value = NULL;
    INIT_LIST_HEAD(&hr->he_list);
    return result;
}

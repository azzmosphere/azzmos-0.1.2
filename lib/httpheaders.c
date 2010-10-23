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
    httpheaders_t *hd = calloc(1,sizeof(httpheaders_t));
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

/*
 * =====================================================================================
 *  httpheaders_init
 *
 *  initlize the default values of the httheaders_t object
 * =====================================================================================
 */
extern void
httpheaders_free(httpheaders_t *hd)
{
    struct list_head *pos, *q;
    httpheaders_t *tmp;
    list_for_each_safe(pos,q, &hd->he_list){
        tmp = list_entry(pos, httpheaders_t, he_list);
        safe_free(hd->he_key);
        safe_free(hd->he_value);
        list_del(pos);
        free(hd);
    }
}

/*
 * =====================================================================================
 *  httpheaders_init
 *
 *  read the headers ptr and create the headers list from the file.  The pointer is one
 *  large memory object which in the case should be ok as headers are not meant to be
 *  to large.
 * =====================================================================================
 */
extern int   
httpheaders_build(httpheaders_t **hd, const char *ptr)
{
}
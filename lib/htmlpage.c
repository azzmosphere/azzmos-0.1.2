/*
 * =====================================================================================
 *
 *       Filename:  htmlpage.c
 *
 *    Description:  HTML page object contains all the attributes that form a HTML
 *                  page including the content.
 *
 *        Version:  1.0
 *        Created:  02/11/2010 10:14:34
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Aaron Spiteri
 *        Company:  
 *
 * =====================================================================================
 */

#include <azzmos/htmlpage.h>

/*
 * =====================================================================================
 * htmlpage_alloc()
 *
 * allocate memory for the htmlpage.
 * =====================================================================================
 */
extern htmlpage_t * 
htmlpage_alloc()
{
    htmlpage_t *page = calloc(1,sizeof(htmlpage_t));
    page->p_url  = NULL;
    page->p_hdr  = NULL;
    page->p_text = NULL;
    page->p_date_ck = page->p_lmod = NULL;
    return page;
}

/*
 * =====================================================================================
 * htmlpage_free_base()
 *
 * free common parts to the HTML page object
 * =====================================================================================
 */
static void
htmlpage_free_base(htmlpage_t *page)
{
    if(!page){
        return;
    }
    safe_free(page->p_text);
    safe_free(page->p_date_ck);
    if(page->p_hdr) {
        httpheaders_free(page->p_hdr);
    }
}

/*
 * =====================================================================================
 * htmlpage_free()
 *
 * release all memory used by the html page object.
 * =====================================================================================
 */
extern void 
htmlpage_free(htmlpage_t *page)
{
    htmlpage_free_base(page);
    if(page->p_url) {
        uri_free(page->p_url);
    }
    free(page);
    page = NULL;
}

/*
 * =====================================================================================
 * htmlpage_alloc()
 *
 * release all memory used by the html object except for the uri object
 * =====================================================================================
 */
extern void
htmlpage_free_nu(htmlpage_t *page)
{
    html_free_page(page);
    free(page);
    page = NULL;
}

/*
 * =====================================================================================
 * html_set_hdr()
 *
 * set the headers for a page, given the headers stream file.
 * =====================================================================================
 */
extern int          
html_set_hdr(htmlpage_t **page, FILE *stream)
{
    int error = 0;
    htmlpage_t *p = *(page);
    
    p->p_hdr = httpheaders_alloc();
    if(!p->p_hdr){
        error = errno;
    }
    if(error == 0){
        error = httpheaders_init(&p->p_hdr);
    }
    if(error == 0){
        error = httpheaders_build(&p->p_hdr, stream);
    }
    return error;
}

/*
 * =====================================================================================
 * html_set_url()
 *
 * set the URL using a parsed uri object.  The URI object should be resolved at this 
 * point.
 * =====================================================================================
 */
extern int
html_set_url(htmlpage_t **page, uriobj_t **url)
{
    int error = 0;
    htmlpage_t *p = *(page);

    p->p_url = *(url);
    return error;
}
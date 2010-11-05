/*
 * =====================================================================================
 *
 *       Filename:  htmlpage.h
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

/* #####   HEADER FILE INCLUDES   ################################################### */
#define _AZZMOS_HTMLPAGE_H_
#ifndef __AZZMOS_COMMON_H__
#include <azzmos/common.h>
#endif
#ifndef __AZZMOS_URIOBJ_H__
#include <azzmos/uriobj.h>
#endif
#ifndef __AZZMOS_URIOBJ_H__
#include <azzmos/uriobj.h>
#endif
#ifndef _AZZMOS_HTTPHEADERS_H_
#include <azzmos/httpheaders.h>
#endif

/* ##### TYPE DEFINITIONS ####################################################### */
struct htmlpage_s {
    long           p_id;      /* unique identifer of the page */
    char          *p_text;    /* page content */
    time_t        *p_date_ck; /* date the page was checked */
    time_t        *p_lmod;    /* date the page was last modified */     
    uriobj_t      *p_url;     /* URL for this page */
    httpheaders_t *p_hdr;     /* URL headers for this page */
} typedef htmlpage_t;

/* ##### EXPORTED FUNCTIONS ##################################################### */
extern htmlpage_t * htmlpage_alloc();
extern void         htmlpage_free(htmlpage_t *page);
extern void         htmlpage_free_nu(htmlpage_t *page);
extern int          html_set_hdr(htmlpage_t **page, FILE *stream);
extern int          html_set_text(htmlpage_t **page, FILE *stream);
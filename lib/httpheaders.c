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
        safe_free(tmp->he_key);
        safe_free(tmp->he_value);
        list_del(pos);
        free(tmp);
    }
    if(hd){
        safe_free(hd->he_key);
        safe_free(hd->he_value);
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
static int
httpheaders_add(httpheaders_t **hd, const char *key, const char *value, bool first)
{
    httpheaders_t *h = *(hd), *tmp;
    
    if(first){
        h->he_key   = URI_CP_PTWS(key);
        h->he_value = URI_CP_PTWS(value);
        return 0;
    }
    tmp = httpheaders_alloc();
    if(!tmp){
        return errno;
    }
    tmp->he_key   = URI_CP_PTWS(key);
    tmp->he_value = URI_CP_PTWS(value);
    list_add(&tmp->he_list, &h->he_list);
    return 0;
}

extern int   
httpheaders_build(httpheaders_t **hd, FILE *ifile)
{
    char *line = malloc(BUFSIZ),
         *val  = NULL,
         *key  = NULL,
         *buf  = NULL;
    int   len  = 0, result = 0;
    bool  first = true;
    httpheaders_t *h = *(hd);
    
    /* get past the HTTP response code. */
    line = fgets(line,BUFSIZ,ifile);
    if(strncasecmp(line,"http",4) == 0){
        line = fgets(line,BUFSIZ,ifile);
    }
    do{
        if(!line || !line[0] || (line[0] == '\r' && line[1] == '\n') || line[0] == '\n'){
            break;
        }
        buf = strsep(&line,":");
        key = URI_CP_PT(buf);
        buf = strsep(&line,":");
        val = URI_CP_PT(buf);
        while(line != NULL){
            len = (strlen(line) + 2 + strlen(val));
            val = realloc(val,len);
            val = strncat(val,":",(len - 1));
            buf = strsep(&line,":");
            if(buf){
                val = strncat(val,buf,(len -1));
            }
        }
        safe_free(line);
        line = NULL;
        line = malloc(BUFSIZ);
        line = fgets(line,BUFSIZ,ifile);
        
        /* add follow on lines to value */
        while(line && isspace(line[0])){
            len = (strlen(line) + 1 + strlen(val));
            val = realloc(val,len);
            val = strncat(val,strdup(line),(len -1));
            safe_free(line);
            line = NULL;
            line = malloc(BUFSIZ);
            line = fgets(line,BUFSIZ,ifile);
        }
        result = httpheaders_add(&h, key, val, first);
        first = false;
        safe_free(key);
        safe_free(val);
        safe_free(line);
        key  = NULL;
        val  = NULL;
    }while(feof(ifile) == 0);
    return result;
}

/*
 * =====================================================================================
 *  httpheader_get_val
 *
 *  return the value in headers object for the header with the key value of key.
 * =====================================================================================
 */
extern char *
httpheader_get_val(const httpheaders_t *hd, const char *key)
{
    char *value = NULL;
    struct list_head *pos;
    httpheaders_t *tmp;
    int len = strlen(key);
    
    /*
     * lets see if we get lucky.
     */
    if(strncasecmp(key,hd->he_key,len) == 0){
        value = URI_CP_PT(hd->he_value);
    }
    else {
        list_for_each(pos, &hd->he_list){
            tmp = list_entry(pos, httpheaders_t, he_list);
            if(strncasecmp(key,tmp->he_key,len) == 0){
                value = URI_CP_PT(tmp->he_value);
                break;
            }
        }
    }
    return value;
}
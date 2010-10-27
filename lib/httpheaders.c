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
httpheaders_add(httpheaders_t **hd, const char *key, const char *value, bool *first)
{
    httpheaders_t *h = *(hd), *tmp;
    
    if(*(first)){
        h->he_key   = URI_CP_PT(key);
        h->he_value = URI_CP_PT(value);
        *(first) = false;
        return 0;
    }
    tmp = httpheaders_alloc();
    if(!tmp){
        return errno;
    }
    tmp->he_key   = URI_CP_PT(key);
    tmp->he_value = URI_CP_PT(value);
    list_add(&h->he_list, &tmp->he_list);
    return 0;
}

extern int   
httpheaders_build(httpheaders_t **hd, FILE *ifile)
{
    char *line = malloc(BUFSIZ),
         *buf  = NULL,
         *key  = NULL;
    int   len  = 0, result = 0;
    bool  first = true;
    
    /*
     * get past the HTTP response code.
     **/
    line = fgets(line,BUFSIZ,ifile);
    if(strncasecmp(line,"http",4) == 0){
        safe_free(line);
        line = fgets(line,BUFSIZ,ifile);
    }
    line = realloc(line,(strlen(line) + 1));
    while(feof(ifile) != 0){
        if(!line && ferror(ifile)){
            result = ferror(ifile);
            break;
        }
        
        /*
         * if we have a null or a blank line then it is time for the
         * program to end.
         **/
        else if(!line || !line[0] || (line[0] == '\r' && line[1] == '\n') || line[0] == '\n'){
            break;
        }
        
        /*
         * add to the buffer,  any carry on headers
         **/
        if(isspace(line[0])){
            len = strlen(buf) + strlen(line);
            if(len > BUFSIZ){
                result = E2BIG;
                break;
            }
            len ++;
            buf = realloc(buf,len);
            buf = strncat(buf,line,len);
        }
        else{
            if(key){
                result = httpheaders_add(hd, key, buf, &first);
                if(result){
                    break;
                }
            }
            safe_free(key);
            safe_free(buf);
            buf = strdup(line);
            key = strsep(&buf,":");
        }
        
        line = fgets(line,BUFSIZ,ifile);
        line = realloc(line,(strlen(line) + 1));        
    }
    
    /*
     * on a error clean up and return.
     **/
    if(result && *(hd)){
        httpheaders_free(*hd);
    }
    safe_free(key);
    safe_free(buf);
    return result;
}
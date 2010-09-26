/*
 * =====================================================================================
 *
 *       Filename:  azzmos-fe.h
 *
 *    Description: This file is the front end header for the libazzmos routines.
 *                 It should be called stead of calling each header directly. 
 *
 *        Version:  1.0
 *        Created:  19/09/2010 13:46:33
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Aaron Spiteri
 *        Company:  
 *
 * =====================================================================================
 */

/* #####   HEADER FILE INCLUDES   ################################################### */
#define AZZMOS_FE_H
/**************************************************************************************
 * It is very unlikly that the common header is not called by one of the sub headers.
 * but include it hear because it will do no harm if we do.
 **************************************************************************************
 */
#ifndef __AZZMOS_COMMON_H__
#include <azzmos/common.h>
#endif

/**************************************************************************************
 * The utilities class provides fundemental miscelaneous functions, such as reset_file
 * and usplice.
 **************************************************************************************
 */
#ifndef _AZZMOS_UTILS_H_
#include <azzmos/utils.h>
#endif

/**************************************************************************************
 * uriobj is the base class for just about everything to with the downloader or indexer.
 * it should called first so that other classes can use it.
 **************************************************************************************
 */
#ifndef __AZZMOS_URIOBJ_H__
#include <azzmos/uriobj.h>
#endif

/**************************************************************************************
 * uriobj is the base class for just about everything to with the downloader or indexer.
 * it should called first so that other classes can use it.
 **************************************************************************************
 */
#ifndef _AZZMOS_REGEXPR_H_
#include <azzmos/regexpr.h>
#endif

/**************************************************************************************
 * uriparse is used to parse the URI object, this includes getting sections of the
 * path.  It does not parse the authority section.  This is done by the normalization
 * object.
 **************************************************************************************
 */
#ifndef  __AZZMOS_URIPARSE_H__
#include <azzmos/uriparse.h>
#endif

/**************************************************************************************
 * urinorm controls the normalization functions. It checks the various components of 
 * the URI for illegal characters, normalization should be performed after parsing but
 * before translation of the reference components.
 **************************************************************************************
 */
#ifndef __AZZMOS_URINORM_H__
#include <azzmos/urinorm.h>
#endif
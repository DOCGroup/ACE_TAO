// -*- C++ -*-

//=============================================================================
/**
 *  @file debug.h
 *
 *  $Id$
 *
 *  @author DOC Group
 */
//=============================================================================

#ifndef TAO_DEBUG_H
#define TAO_DEBUG_H

#include /**/ "ace/pre.h"

#include /**/ "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "tao/Versioned_Namespace.h"
#include "ace/Log_Category.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// These are global to simplify is use by other code, very much in
// particular by getopt and related argument-parsing code
//
// THREADING NOTE: don't set them except in an unthreaded environment
// such as process initialization.  They're treated as immutable.

// 0 to ??; higher == more
extern TAO_Export unsigned int TAO_debug_level;

// debug messages on (1) or off (0)
extern TAO_Export unsigned int TAO_orbdebug;

extern TAO_Export ACE_Log_Category TAO_log_category;

TAO_END_VERSIONED_NAMESPACE_DECL


#if defined (ACE_NLOGGING)
#if !defined (TAOLIB_HEX_DUMP)
# define TAOLIB_HEX_DUMP(X) do {} while (0)
#endif
#if !defined (TAOLIB_ERROR_RETURN)
# define TAOLIB_ERROR_RETURN(X, Y) return (Y)
#endif
#if !defined (TAOLIB_ERROR)
# define TAOLIB_ERROR(X) do {} while (0)
#endif
#if !defined (TAOLIB_DEBUG)
# define TAOLIB_DEBUG(X) do {} while (0)
#endif
#else
#if !defined (TAOLIB_HEX_DUMP)
#define TAOLIB_HEX_DUMP(X) \
  do { \
    int const __ace_error = ACE_Log_Msg::last_error_adapter (); \
    ACE_Log_Category_TSS *ace___ = TAO_VERSIONED_NAMESPACE_NAME::TAO_log_category.per_thr_obj(); \
    if (ace___ == 0) break;\
    ace___->conditional_set (__FILE__, __LINE__, 0, __ace_error); \
    ace___->log_hexdump X; \
  } while (0)
#endif
#if !defined (TAOLIB_ERROR_RETURN)
#define TAOLIB_ERROR_RETURN(X, Y) \
  do { \
    int const __ace_error = ACE_Log_Msg::last_error_adapter (); \
    ACE_Log_Category_TSS *ace___ = TAO_VERSIONED_NAMESPACE_NAME::TAO_log_category.per_thr_obj(); \
    if (ace___ == 0) break;\
    ace___->conditional_set (__FILE__, __LINE__, Y, __ace_error); \
    ace___->log X; \
    return Y; \
  } while (0)
#endif
#if !defined (TAOLIB_ERROR)
#define TAOLIB_ERROR(X) \
  do { \
    int const __ace_error = ACE_Log_Msg::last_error_adapter (); \
    ACE_Log_Category_TSS *ace___ = TAO_VERSIONED_NAMESPACE_NAME::TAO_log_category.per_thr_obj(); \
    if (ace___ == 0) break;\
    ace___->conditional_set (__FILE__, __LINE__, -1, __ace_error); \
    ace___->log X; \
  } while (0)
#endif
#if !defined (TAOLIB_DEBUG)
#define TAOLIB_DEBUG(X) \
  do { \
    int const __ace_error = ACE_Log_Msg::last_error_adapter (); \
    ACE_Log_Category_TSS *ace___ = TAO_VERSIONED_NAMESPACE_NAME::TAO_log_category.per_thr_obj(); \
    if (ace___ == 0) break;\
    ace___->conditional_set (__FILE__, __LINE__, 0, __ace_error); \
    ace___->log X; \
  } while (0)
#endif
#endif /* ACE_NLOGGING */

#include /**/ "ace/post.h"
#endif /* TAO_DEBUG_H */

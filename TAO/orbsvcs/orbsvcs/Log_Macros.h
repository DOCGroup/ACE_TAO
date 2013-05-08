// $Id$

#ifndef TAO_ORBSVCS_LOG_MACROS_H
#define TAO_ORBSVCS_LOG_MACROS_H
#include /**/ "ace/pre.h"

#include "ace/Log_Category.h"
#include "ace/Log_Msg.h"

#include "tao/Versioned_Namespace.h"

#include "svc_utils_export.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

extern TAO_Svc_Utils_Export ACE_Log_Category ORBSVCS_log_category;

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (ACE_NLOGGING)
#if !defined (ORBSVCS_HEX_DUMP)
# define ORBSVCS_HEX_DUMP(X) do {} while (0)
#endif
#if !defined (ORBSVCS_ERROR_RETURN)
# define ORBSVCS_ERROR_RETURN(X, Y) return (Y)
#endif
#if !defined (ORBSVCS_ERROR)
# define ORBSVCS_ERROR(X) do {} while (0)
#endif
#if !defined (ORBSVCS_DEBUG)
# define ORBSVCS_DEBUG(X) do {} while (0)
#endif
#else
#if !defined (ORBSVCS_HEX_DUMP)
#define ORBSVCS_HEX_DUMP(X) \
  do { \
    int const __ace_error = ACE_Log_Msg::last_error_adapter (); \
    ACE_Log_Category_TSS *ace___ = TAO_VERSIONED_NAMESPACE_NAME::ORBSVCS_log_category.per_thr_obj(); \
    if (ace___ == 0) break;\
    ace___->conditional_set (__FILE__, __LINE__, 0, __ace_error); \
    ace___->log_hexdump X; \
  } while (0)
#endif
#if !defined (ORBSVCS_ERROR_RETURN)
#define ORBSVCS_ERROR_RETURN(X, Y) \
  do { \
    int const __ace_error = ACE_Log_Msg::last_error_adapter (); \
    ACE_Log_Category_TSS *ace___ = TAO_VERSIONED_NAMESPACE_NAME::ORBSVCS_log_category.per_thr_obj(); \
    if (ace___ == 0) return Y;\
    ace___->conditional_set (__FILE__, __LINE__, Y, __ace_error); \
    ace___->log X; \
    return Y; \
  } while (0)
#endif
#if !defined (ORBSVCS_ERROR)
#define ORBSVCS_ERROR(X) \
  do { \
    int const __ace_error = ACE_Log_Msg::last_error_adapter (); \
    ACE_Log_Category_TSS *ace___ = TAO_VERSIONED_NAMESPACE_NAME::ORBSVCS_log_category.per_thr_obj(); \
    if (ace___ == 0) break;\
    ace___->conditional_set (__FILE__, __LINE__, -1, __ace_error); \
    ace___->log X; \
  } while (0)
#endif
#if !defined (ORBSVCS_DEBUG)
#define ORBSVCS_DEBUG(X) \
  do { \
    int const __ace_error = ACE_Log_Msg::last_error_adapter (); \
    ACE_Log_Category_TSS *ace___ = TAO_VERSIONED_NAMESPACE_NAME::ORBSVCS_log_category.per_thr_obj(); \
    if (ace___ == 0) break;\
    ace___->conditional_set (__FILE__, __LINE__, 0, __ace_error); \
    ace___->log X; \
  } while (0)
#endif
#endif /* ACE_NLOGGING */

#include /**/ "ace/post.h"
#endif

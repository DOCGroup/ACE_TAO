// $Id$
/**
 * @file Log_Macros.h
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 *
 * Macros used for logging in CIAO.
 */

#ifndef CIAO_LOG_MACROS_H_
#define CIAO_LOG_MACROS_H_

#include "ace/Log_Category.h"

// default information printed with CIAO logging messages.

#include "CIAO_Logger_Export.h"

#if !defined (CLINFO)
# define CLINFO "(%P|%t) [%M] - %T - "
#endif

extern CIAO_Logger_Export unsigned int CIAO_debug_level;

extern CIAO_Logger_Export ACE_Log_Category CIAO_log_category;

// By default tracing is turned off.
#if !defined (CIAO_NTRACE)
#  define CIAO_NTRACE 1
#endif /* CIAO_NTRACE */

#if (CIAO_NTRACE == 1)
#  if !defined (ACE_NTRACE)
#    define CIAO_TRACE(X) do {} while (0)
#    define CIAO_ENABLE_TRACE() do {} while (0)
#    define CIAO_DISABLE_TRACE() do {} while (0)
#  else
#    if (ACE_NTRACE == 0)
#      error CIAO_TRACE cannot be disabled if ACE_TRACE is enabled
#    else
#      define CIAO_TRACE(X) do {} while (0)
#      define CIAO_ENABLE_TRACE() do {} while (0)
#      define CIAO_DISABLE_TRACE() do {} while (0)
#    endif
#  endif
#else
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define CIAO_TRACE(X) ACE_TRACE_IMPL(X)
#  define CIAO_ENABLE_TRACE() ACE_Trace::start_tracing ()
#  define CIAO_DISABLE_TRACE() ACE_Trace::stop_tracing ()
#  undef CLINFO // Make log messages indent with tracing.
#  define CLINFO "%I(%P|%t) [%M] - %T - "
#  include "ace/Trace.h"
#endif /* CIAO_NTRACE */

#if defined (CIAO_NLOGGING)
# define CIAO_ERROR(L, X) do {} while (0)
# define CIAO_DEBUG(L, X) do {} while (0)
# define CIAO_ERROR_RETURN(L, X, Y) return (Y)
# define CIAO_ERROR_BREAK(L, X) { break; }
#else
# if !defined (CIAO_ERROR)
#  define CIAO_ERROR(L, X) \
  do { \
    if (CIAO_debug_level >= L) \
      { \
        int const __ace_error = ACE_Log_Msg::last_error_adapter (); \
        ACE_Log_Category_TSS *ace___ = CIAO_log_category.per_thr_obj (); \
        if (ace___ == 0) break; \
        ace___->conditional_set (__FILE__, __LINE__, -1, __ace_error); \
        ace___->log X; \
      } \
  } while (0)
#  endif
# if !defined (CIAO_DEBUG)
#  define CIAO_DEBUG(L, X) \
  do { \
    if (CIAO_debug_level >= L) \
      { \
        int const __ace_error = ACE_Log_Msg::last_error_adapter (); \
        ACE_Log_Category_TSS *ace___ = CIAO_log_category.per_thr_obj (); \
        if (ace___ == 0) break; \
        ace___->conditional_set (__FILE__, __LINE__, 0, __ace_error); \
        ace___->log X; \
      } \
  } while (0)
# endif
# if !defined (CIAO_ERROR_RETURN)
#  define CIAO_ERROR_RETURN(L, X, Y) \
  do { \
    if (CIAO_debug_level >= L) \
      { \
        int const __ace_error = ACE_Log_Msg::last_error_adapter (); \
        ACE_Log_Category_TSS *ace___ = CIAO_log_category.per_thr_obj (); \
        if (ace___ == 0) break; \
        ace___->conditional_set (__FILE__, __LINE__, Y, __ace_error); \
        ace___->log X; \
      } \
    return Y; \
  } while (0)
# endif
# if !defined (CIAO_ERROR_BREAK)
#  define CIAO_ERROR_BREAK(L, X) { CIAO_ERROR (L, X); break; }
# endif
#endif


#endif

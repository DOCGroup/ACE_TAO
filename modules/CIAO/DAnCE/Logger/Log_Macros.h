/**
 * @file Log_Macros.h
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * // $Id$
 * Macros used for logging in DAnCE
 */


#ifndef DANCE_LOG_MACROS_H_
#define DANCE_LOG_MACROS_H_

#include "DAnCE_Logger_Export.h"

#if !defined (DLINFO)
# define DLINFO ACE_TEXT("(%P|%t) [%M] - %T - ")
#endif

extern DAnCE_Logger_Export unsigned int DAnCE_debug_level;

// By default tracing is turned off.
#if !defined (DANCE_NTRACE)
#  define DANCE_NTRACE 1
#endif /* DANCE_NTRACE */

#if (DANCE_NTRACE == 1)
#  if !defined (ACE_NTRACE)
#    define DANCE_TRACE(X) do {} while (0)
#    define DANCE_ENABLE_TRACE() do {} while (0)
#    define DANCE_DISABLE_TRACE() do {} while (0)
#  else
#    if (ACE_NTRACE == 0)
#      error DANCE_TRACE cannot be disabled if ACE_TRACE is enabled
#    else
#      define DANCE_TRACE(X) do {} while (0)
#      define DANCE_ENABLE_TRACE() do {} while (0)
#      define DANCE_DISABLE_TRACE() do {} while (0)
#    endif
#  endif
#else
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define DANCE_TRACE(X) ACE_TRACE_IMPL (X)
#  define DANCE_ENABLE_TRACE() ACE_Trace::start_tracing ()
#  define DANCE_DISABLE_TRACE() ACE_Trace::stop_tracing ()
#  undef DLINFO // Make log messages indent with tracing.
#  define DLINFO ACE_TEXT("%I(%P|%t) [%M] - %T - ")
#  include "ace/Trace.h"
#endif /* DANCE_NTRACE */

#if defined (DANCE_NLOGGING)
# define DANCE_ERROR(L, X) do {} while (0)
# define DANCE_DEBUG(L, X) do {} while (0)
#define DANCE_ERROR_RETURN(L, X, Y) return (Y)
#define DANCE_ERROR_BREAK(L, X) { break; }
#else
# if !defined (DANCE_ERROR)
#  define DANCE_ERROR(L, X) \
  do { \
    if (DAnCE_debug_level >= L) \
      { \
        int const __ace_error = ACE_Log_Msg::last_error_adapter (); \
        ACE_Log_Msg *ace___ = ACE_Log_Msg::instance ();               \
        ace___->conditional_set (__FILE__, __LINE__, -1, __ace_error); \
        ace___->log X; \
      } \
  } while (0)
#  endif
# if !defined (DANCE_DEBUG)
#  define DANCE_DEBUG(L, X) \
  do { \
    if (DAnCE_debug_level >= L) \
      { \
        int const __ace_error = ACE_Log_Msg::last_error_adapter (); \
        ACE_Log_Msg *ace___ = ACE_Log_Msg::instance (); \
        ace___->conditional_set (__FILE__, __LINE__, 0, __ace_error); \
        ace___->log X; \
      } \
  } while (0)
# endif
# if !defined (DANCE_ERROR_RETURN)
#  define DANCE_ERROR_RETURN(L, X, Y) \
  do { \
    if (DAnCE_debug_level >= L) \
      { \
        int const __ace_error = ACE_Log_Msg::last_error_adapter (); \
        ACE_Log_Msg *ace___ = ACE_Log_Msg::instance (); \
        ace___->conditional_set (__FILE__, __LINE__, Y, __ace_error); \
        ace___->log X; \
        return Y; \
      } \
  } while (0)
# endif
# if !defined (DANCE_ERROR_BREAK)
#  define DANCE_ERROR_BREAK(L, X) { DANCE_ERROR (L, X); break; }
# endif
#endif


#endif

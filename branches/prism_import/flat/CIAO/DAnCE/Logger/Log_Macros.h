/**
 * @file Log_Macros.h
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 *
 * Macros used for logging in DAnCE
 */

#ifndef DANCE_LOG_MACROS_H_
#define DANCE_LOG_MACROS_H_

#define DLINFO "(%P|%t) [%M] - %T - "

#if (DANCE_NTRACE == 1)
#  if !defined (ACE_NTRACE)
#    define DANCE_TRACE(X)
#  else
#    if (ACE_NTRACE == 0)
#      error DANCE_TRACE cannot be disabled if ACE_TRACE is enabled
#    else
#      define DANCE_TRACE(X)
#    endif
#  endif
#else
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define DANCE_TRACE(X) ACE_TRACE_IMPL(DLINFO X)
#  include "ace/Trace.h"
#endif /* DANCE_NTRACE */

#if defined (DANCE_NLOGGING)
# define DANCE_ERROR(X) do {} while (0)
# define DANCE_DEBUG(X) do {} while (0)
#define DANCE_ERROR_RETURN(X, Y) return (Y)
#define DANCE_ERROR_BREAK(X) { break; }
#else
# if !defined (DANCE_ERROR)
#  define DANCE_ERROR(X) \
  do { \
    int __ace_error = ACE_Log_Msg::last_error_adapter (); \
    ACE_Log_Msg *ace___ = ACE_Log_Msg::instance ();               \
    ace___->conditional_set (__FILE__, __LINE__, -1, __ace_error); \
    ace___->log X; \
  } while (0)
#  endif
# if !defined (DANCE_DEBUG)
#  define DANCE_DEBUG(X) \
  do { \
    int __ace_error = ACE_Log_Msg::last_error_adapter (); \
    ACE_Log_Msg *ace___ = ACE_Log_Msg::instance (); \
    ace___->conditional_set (__FILE__, __LINE__, 0, __ace_error); \
    ace___->log X; \
  } while (0)
# endif
# if !defined (DANCE_ERROR_RETURN)
#  define DANCE_ERROR_RETURN(X, Y) \
  do { \
    int __ace_error = ACE_Log_Msg::last_error_adapter (); \
    ACE_Log_Msg *ace___ = ACE_Log_Msg::instance (); \
    ace___->conditional_set (__FILE__, __LINE__, Y, __ace_error); \
    ace___->log X; \
    return Y; \
  } while (0)
# endif
# if !defined (DANCE_ERROR_BREAK)
#  define DANCE_ERROR_BREAK(X) { DANCE_ERROR (X); break; }
# endif
#endif


#endif

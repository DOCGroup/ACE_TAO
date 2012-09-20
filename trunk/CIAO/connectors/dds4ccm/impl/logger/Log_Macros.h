// $Id$
/**
 * @file Log_Macros.h
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 *
 * Macros used for logging in DDS4CCM
 */

#ifndef DDS4CCM_LOG_MACROS_H_
#define DDS4CCM_LOG_MACROS_H_

// default information printed with CIAO logging messages.
#include "DDS4CCM_Logger_Export.h"
#include "ace/Log_Msg.h"

#if !defined (DDS4CCM_INFO)
#  define DDS4CCM_INFO "(%P|%t) [%M] - %T - "
#endif

// Defines for log levels
#define DDS4CCM_LOG_LEVEL_ERROR           1
#define DDS4CCM_LOG_LEVEL_CAST_ERROR      2
#define DDS4CCM_LOG_LEVEL_DDS_NIL_RETURN  3
#define DDS4CCM_LOG_LEVEL_4               4 //unused
#define DDS4CCM_LOG_LEVEL_5               5 //unused
#define DDS4CCM_LOG_LEVEL_ACTION          6
#define DDS4CCM_LOG_LEVEL_UNIMP_ACTION    7
#define DDS4CCM_LOG_LEVEL_ACTION_STARTING 8
#define DDS4CCM_LOG_LEVEL_CAST_SUCCESSFUL 9
#define DDS4CCM_LOG_LEVEL_DDS_STATUS     10

extern DDS4CCM_Logger_Export unsigned int DDS4CCM_debug_level;

// By default tracing is turned off.
#if !defined (DDS4CCM_NTRACE)
#  define DDS4CCM_NTRACE 1
#endif /* DDS4CCM_NTRACE */

#if (DDS4CCM_NTRACE == 1)
#  if !defined (ACE_NTRACE)
#    define DDS4CCM_TRACE(X) do {} while (0)
#    define DDS4CCM_ENABLE_TRACE() do {} while (0)
#    define DDS4CCM_DISABLE_TRACE() do {} while (0)
#  else
#    if (ACE_NTRACE == 0)
#      error DDS4CCM_TRACE cannot be disabled if ACE_TRACE is enabled
#    else
#      define DDS4CCM_TRACE(X) do {} while (0)
#      define DDS4CCM_ENABLE_TRACE() do {} while (0)
#      define DDS4CCM_DISABLE_TRACE() do {} while (0)
#    endif
#  endif
#else
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define DDS4CCM_TRACE(X) ACE_TRACE_IMPL(X)
#  define DDS4CCM_ENABLE_TRACE() ACE_Trace::start_tracing ()
#  define DDS4CCM_DISABLE_TRACE() ACE_Trace::stop_tracing ()
#  undef DDS4CCM_INFO // Make log messages indent with tracing.
#  define DDS4CCM_INFO "%I(%P|%t) [%M] - %T - "
#  include "ace/Trace.h"
#endif /* DDS4CCM_NTRACE */

#if defined (DDS4CCM_NLOGGING)
# define DDS4CCM_ERROR(L, X) do {} while (0)
# define DDS4CCM_DEBUG(L, X) do {} while (0)
# define DDS4CCM_PRINT_INTERNAL_EXCEPTION(L, E, X) do {} while (0)
# define DDS4CCM_PRINT_CORBA_EXCEPTION(L, E, X) do {} while (0)
# define DDS4CCM_ERROR_RETURN(L, X, Y) return (Y)
# define DDS4CCM_ERROR_BREAK(L, X) { break; }
#else
# if !defined (DDS4CCM_ERROR)
#  define DDS4CCM_ERROR(L, X) \
  do { \
    if (DDS4CCM_debug_level >= L) \
      { \
        int const __ace_error = ACE_Log_Msg::last_error_adapter (); \
        ACE_Log_Msg *ace___ = ACE_Log_Msg::instance ();               \
        ace___->conditional_set (__FILE__, __LINE__, -1, __ace_error); \
        ace___->log X; \
      } \
  } while (0)
#  endif

# if !defined (DDS4CCM_DEBUG)
#  define DDS4CCM_DEBUG(L, X) \
  do { \
    if (DDS4CCM_debug_level >= L) \
      { \
        int const __ace_error = ACE_Log_Msg::last_error_adapter (); \
        ACE_Log_Msg *ace___ = ACE_Log_Msg::instance (); \
        ace___->conditional_set (__FILE__, __LINE__, 0, __ace_error); \
        ace___->log X; \
      } \
  } while (0)
# endif

# if !defined (DDS4CCM_PRINT_INTERNAL_EXCEPTION)
#  define DDS4CCM_PRINT_INTERNAL_EXCEPTION(L, E, X) \
  do { \
    if (DDS4CCM_debug_level >= L) \
      { \
        ACE_CString msg (X);\
        msg += " - Caught CCM_DDS internal exception: error <";\
        msg += E; \
        msg += ">\n";\
        DDS4CCM_ERROR (L, (LM_ERROR, msg.c_str ()));\
      } \
  } while (0)
# endif
# if !defined (DDS4CCM_PRINT_CORBA_EXCEPTION)
#  define DDS4CCM_PRINT_CORBA_EXCEPTION(L, E, X) \
  do { \
    if (DDS4CCM_debug_level >= L) \
      { \
        ACE_CString msg (X);\
        msg += " - Caught CORBA exception.\n";\
        DDS4CCM_ERROR (L, (LM_ERROR, msg.c_str ()));\
        E._tao_print_exception (X);\
      } \
  } while (0)
# endif
# if !defined (DDS4CCM_PRINT_DEBUG_CORBA_EXCEPTION)
#  define DDS4CCM_PRINT_DEBUG_CORBA_EXCEPTION(L, E, X) \
  do { \
    if (DDS4CCM_debug_level >= L) \
      { \
        ACE_CString msg (X);\
        msg += " - Caught CORBA exception.\n";\
        DDS4CCM_DEBUG (L, (LM_DEBUG, msg.c_str ()));\
        E._tao_print_exception (X);\
      } \
  } while (0)
# endif
# if !defined (DDS4CCM_ERROR_RETURN)
#  define DDS4CCM_ERROR_RETURN(L, X, Y) \
  do { \
    if (DDS4CCM_debug_level >= L) \
      { \
        int const __ace_error = ACE_Log_Msg::last_error_adapter (); \
        ACE_Log_Msg *ace___ = ACE_Log_Msg::instance (); \
        ace___->conditional_set (__FILE__, __LINE__, Y, __ace_error); \
        ace___->log X; \
      } \
    return Y; \
  } while (0)
# endif
# if !defined (DDS4CCM_ERROR_BREAK)
#  define DDS4CCM_ERROR_BREAK(L, X) { DDS4CCM_ERROR (L, X); break; }
# endif
#endif


#endif

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

// Defines for logging levels

/// Used for errors that cause the fatal shutdown of any portion
/// of the infrastructure.
#define DANCE_LOG_EMERGENCY            0
/// Used for deployment errors at the point the error
/// exits the process in question, or when a decision is made to
/// cause the deployment to fail.
#define DANCE_LOG_TERMINAL_ERROR       1
/// Used for non-fatal deployment errors that do not cause deployment
/// failure.
#define DANCE_LOG_NONFATAL_ERROR       2
/// Used to log detailed error information at the point of failure
#define DANCE_LOG_ERROR                3
/// used to indicate that a questionable situation that doesn't cause
/// deployment failure, but can cause undefined conditions.
#define DANCE_LOG_WARNING              4
/// Used to indicate that a ``significant'' deployment event has completed.
/// A 'major' event will depend heavily on the context in which it
/// occurs.  Major will include instance deployments/teardowns,
/// process spawns, etc.
#define DANCE_LOG_MAJOR_EVENT          5
/// Used to inficate a ``minor'' deployment event has completed.
/// Minor events will usually change the state of a deployed instance,
/// e.g. lifecycle changes (passivation, configuration complete),
/// interceptor invocations, etc.
#define DANCE_LOG_MINOR_EVENT          6
/// Used to trace significant actions within major/minor events.  This
/// will usually include starts for major/minor events.
#define DANCE_LOG_EVENT_TRACE          7
/// Used to display important configuration information that impacts
/// the deployment process.
#define DANCE_LOG_MAJOR_DEBUG_INFO     8
/// The following two statements are used to closely trace the
/// execution of DAnCE code.  If in doubt, a message should go into
/// these categories.  The difference between a TRACE and a
/// DETAILED_TRACE will be largely a judgement call.  These messages
/// should be emitted using DANCE_LOG_TRACE (X, Y).
#define DANCE_LOG_TRACE                9
#define DANCE_LOG_DETAILED_TRACE     10

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
# if !defined (DANCE_LOG_TRACE)
#  define DANCE_LOG_TRACE(L, X) \
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

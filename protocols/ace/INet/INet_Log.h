/**
 * @file INet_Log.h
 * @author Martin Corino <mcorino@remedy.nl>
 * // $Id$
 * Macros used for logging in INet
 */


#ifndef INET_LOG_MACROS_H_
#define INET_LOG_MACROS_H_

#include "ace/INet/INet_Export.h"

#if !defined (DLINFO)
# define DLINFO ACE_TEXT("(%P|%t) [%M] - %T - ")
#endif

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
  {
    class ACE_INET_Export INet_Log
    {
      public:
        INet_Log () {}

        static unsigned int debug_level_;

        static unsigned int Initializer (void);
    };
  }

ACE_END_VERSIONED_NAMESPACE_DECL

// By default tracing is turned on in debugmode, off otherwise
#if !defined (INET_NTRACE)
# if defined (NDEBUG)
#   define INET_NTRACE 1
# else
#   define INET_NTRACE 0
# endif
#endif /* INET_NTRACE */

#if (INET_NTRACE == 1)
#  if !defined (ACE_NTRACE)
#    define INET_TRACE(X) do {} while (0)
#    define INET_ENABLE_TRACE() do {} while (0)
#    define INET_DISABLE_TRACE() do {} while (0)
#  else
#    if (ACE_NTRACE == 0)
#      error INET_TRACE cannot be disabled if ACE_TRACE is enabled
#    else
#      define INET_TRACE(X) do {} while (0)
#      define INET_ENABLE_TRACE() do {} while (0)
#      define INET_DISABLE_TRACE() do {} while (0)
#    endif
#  endif
#else
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define INET_TRACE(X) ACE_TRACE_IMPL (X)
#  define INET_ENABLE_TRACE() ACE_Trace::start_tracing ()
#  define INET_DISABLE_TRACE() ACE_Trace::stop_tracing ()
#  undef DLINFO // Make log messages indent with tracing.
#  define DLINFO ACE_TEXT("%I(%P|%t) [%M] - %T - ")
#  include "ace/Trace.h"
#endif /* INET_NTRACE */

#if defined (INET_NLOGGING)
# define INET_ERROR(L, X) do {} while (0)
# define INET_DEBUG(L, X) do {} while (0)
#define INET_ERROR_RETURN(L, X, Y) return (Y)
#define INET_ERROR_BREAK(L, X) { break; }
#else
# if !defined (INET_ERROR)
#  define INET_ERROR(L, X) \
  do { \
    if (::ACE::INet_Log::debug_level_ >= L) \
      { \
        int const __ace_error = ACE_Log_Msg::last_error_adapter (); \
        ACE_Log_Msg *ace___ = ACE_Log_Msg::instance ();               \
        ace___->conditional_set (__FILE__, __LINE__, -1, __ace_error); \
        ace___->log X; \
      } \
  } while (0)
#  endif
# if !defined (INET_DEBUG)
#  define INET_DEBUG(L, X) \
  do { \
    if (::ACE::INet_Log::debug_level_ >= L) \
      { \
        int const __ace_error = ACE_Log_Msg::last_error_adapter (); \
        ACE_Log_Msg *ace___ = ACE_Log_Msg::instance (); \
        ace___->conditional_set (__FILE__, __LINE__, 0, __ace_error); \
        ace___->log X; \
      } \
  } while (0)
# endif
# if !defined (INET_HEX_DUMP)
#define INET_HEX_DUMP(L, X) \
  do { \
    if (::ACE::INet_Log::debug_level_ >= L) \
      { \
        int const __ace_error = ACE_Log_Msg::last_error_adapter (); \
        ACE_Log_Msg *ace___ = ACE_Log_Msg::instance (); \
        ace___->conditional_set (__FILE__, __LINE__, 0, __ace_error); \
        ace___->log_hexdump X; \
      } \
  } while (0)
#endif
# if !defined (INET_ERROR_RETURN)
#  define INET_ERROR_RETURN(L, X, Y) \
  do { \
    if (::ACE::INet_Log::debug_level_ >= L) \
      { \
        int const __ace_error = ACE_Log_Msg::last_error_adapter (); \
        ACE_Log_Msg *ace___ = ACE_Log_Msg::instance (); \
        ace___->conditional_set (__FILE__, __LINE__, Y, __ace_error); \
        ace___->log X; \
        return Y; \
      } \
  } while (0)
# endif
# if !defined (INET_ERROR_BREAK)
#  define INET_ERROR_BREAK(L, X) { INET_ERROR (L, X); break; }
# endif
#endif


#endif

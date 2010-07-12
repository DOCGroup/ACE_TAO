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

#include "ciao/Logger/Log_Macros.h"
#include "dds4ccm/impl/dds4ccm_dds_impl_export.h"

#if !defined (CLINFO)
#  define CLINFO "(%P|%t) [%M] - %T - "
#endif

extern DDS4CCM_DDS_IMPL_Export unsigned int DDS4CCM_debug_level;

// By default tracing is turned off.
#if !defined (DDS4CCM_NTRACE)
#  define DDS4CCM_NTRACE 1
#endif /* DDS4CCM_NTRACE */

#if (DDS4CCM_NTRACE == 1)
#  if !defined (ACE_NTRACE)
#    define DDS4CCM_TRACE(X) do {} while (0)
#    define DDS4CCM_ENABLE_TRACE(X) do {} while (0)
#    define DDS4CCM_DISABLE_TRACE(X) do {} while (0)
#  else
#    if (ACE_NTRACE == 0)
#      error DDS4CCM_TRACE cannot be disabled if ACE_TRACE is enabled
#    else
#      define DDS4CCM_TRACE(X) do {} while (0)
#      define DDS4CCM_ENABLE_TRACE(X) do {} while (0)
#      define DDS4CCM_DISABLE_TRACE(X) do {} while (0)
#    endif
#  endif
#else
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define DDS4CCM_TRACE(X) ACE_TRACE_IMPL(X)
#  define DDS4CCM_ENABLE_TRACE() ACE_Trace::start_tracing ()
#  define DDS4CCM_DISABLE_TRACE() ACE_Trace::stop_tracing ()
#  undef CLINFO // Make log messages indent with tracing.
#  define CLINFO "%I(%P|%t) [%M] - %T - "
#  include "ace/Trace.h"
#endif /* DDS4CCM_NTRACE */

#if defined (DDS4CCM_NLOGGING)
# define DDS4CCM_ERROR(L, X) do {} while (0)
# define DDS4CCM_DEBUG(L, X) do {} while (0)
# define DDS4CCM_PRINT_INTERNAL_EXCEPTION (L, E, X) do {} while (0)
# define DDS4CCM_PRINT_CORBA_EXCEPTION (L, E, X) do {} while (0)
# define DDS4CCM_ERROR_RETURN(L, X, Y) return (Y)
# define DDS4CCM_ERROR_BREAK(L, X) { break; }
#else
# if !defined (DDS4CCM_ERROR)
#  define DDS4CCM_ERROR(L, X) \
  do { \
    if (CIAO_debug_level >= L) \
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
    if (CIAO_debug_level >= L) \
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
    if (CIAO_debug_level >= L) \
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
    if (CIAO_debug_level >= L) \
      { \
        ACE_CString msg (X);\
        msg += " - Caught CORBA exception.\n";\
        DDS4CCM_ERROR (L, (LM_ERROR, msg.c_str ()));\
        E._tao_print_exception (X);\
      } \
  } while (0)
# endif
# if !defined (DDS4CCM_ERROR_RETURN)
#  define DDS4CCM_ERROR_RETURN(L, X, Y) \
  do { \
    if (CIAO_debug_level >= L) \
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

// -*- C++ -*-

//==========================================================================
/**
 *  @file   config-all.h
 *
 *  @author Doug Schmidt <d.schmidt@vanderbilt.edu>
 *  @author Jesper S. M|ller<stophph@diku.dk>
 *  @author and a cast of thousands...
 */
//==========================================================================

#ifndef ACE_CONFIG_ALL_H
#define ACE_CONFIG_ALL_H

#include /**/ "ace/pre.h"

#include /**/ "ace/config-lite.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// This is used to indicate that a platform doesn't support a
// particular feature.
#if defined ACE_HAS_VERBOSE_NOTSUP
  // Print a console message with the file and line number of the
  // unsupported function.
# include "ace/OS_NS_stdio.h"
# define ACE_NOTSUP_RETURN(FAILVALUE) do { errno = ENOTSUP; ACE_OS::fprintf (stderr, ACE_TEXT ("ACE_NOTSUP: %s, line %d\n"), __FILE__, __LINE__); return FAILVALUE; } while (0)
# define ACE_NOTSUP do { errno = ENOTSUP; ACE_OS::fprintf (stderr, ACE_TEXT ("ACE_NOTSUP: %s, line %d\n"), __FILE__, __LINE__); return; } while (0)
#else /* ! ACE_HAS_VERBOSE_NOTSUP */
# define ACE_NOTSUP_RETURN(FAILVALUE) do { errno = ENOTSUP ; return FAILVALUE; } while (0)
# define ACE_NOTSUP do { errno = ENOTSUP; return; } while (0)
#endif /* ! ACE_HAS_VERBOSE_NOTSUP */

// ----------------------------------------------------------------

# define ACE_TRACE_IMPL(X) ACE_Trace ____ (ACE_TEXT (X), __LINE__, ACE_TEXT (__FILE__))

// By default tracing is turned off.
#if !defined (ACE_NTRACE)
#  define ACE_NTRACE 1
#endif /* ACE_NTRACE */

#if (ACE_NTRACE == 1)
#  define ACE_TRACE(X)
#else
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define ACE_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* ACE_NTRACE */

// By default we perform no tracing on the OS layer, otherwise the
// coupling between the OS layer and Log_Msg is too tight.  But the
// application can override the default if they wish to.
#if !defined (ACE_OS_NTRACE)
#  define ACE_OS_NTRACE 1
#endif /* ACE_OS_NTRACE */

#if (ACE_OS_NTRACE == 1)
#  define ACE_OS_TRACE(X)
#else
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define ACE_OS_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* ACE_OS_NTRACE */

#if !defined (ACE_TEMPLATES_REQUIRE_SOURCE)
# define ACE_TEMPLATES_REQUIRE_SOURCE
#endif

#if !defined (ACE_HAS_MONITOR_FRAMEWORK)
# define ACE_HAS_MONITOR_FRAMEWORK 1
#endif

#if !defined (ACE_HAS_SENDFILE)
# define ACE_HAS_SENDFILE 0
#endif

#if !defined (ACE_HAS_MONITOR_POINTS)
# define ACE_HAS_MONITOR_POINTS 0
#endif

// For all compilers we require the std library to be in the std namespace
#if !defined (ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB)
#define ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB 1
#endif

// We require that the standard C++ library is support
#if !defined (ACE_HAS_STANDARD_CPP_LIBRARY)
# define ACE_HAS_STANDARD_CPP_LIBRARY 1
#endif

// Define ACE_NEW_THROWS_EXCEPTIONS for all compilers, we
// expect all compilers to do so
#if !defined (ACE_NEW_THROWS_EXCEPTIONS)
# define ACE_NEW_THROWS_EXCEPTIONS
#endif /* ACE_NEW_THROWS_EXCEPTIONS */

// Define ACE_HAS_NEW_NOTHROW for all compilers, we
// except all compilers to support this
#if !defined (ACE_HAS_NEW_NOTHROW)
# define ACE_HAS_NEW_NOTHROW
#endif /* ACE_HAS_NEW_NOTHROW */

// We require the platform to have a string class
#if !defined (ACE_HAS_STRING_CLASS)
# define ACE_HAS_STRING_CLASS
#endif /* ACE_HAS_STRING_CLASS */

// ACE 7.0.3 renamed this macro, defining the old name for
// backwards compatibility
#if defined (ACE_HAS_WIN32_STRUCTURED_EXCEPTIONS)
# define ACE_HAS_WIN32_STRUCTURAL_EXCEPTIONS
#endif /* ACE_HAS_WIN32_STRUCTURED_EXCEPTIONS */


// These includes are here to avoid circular dependencies.
// Keep this at the bottom of the file.  It contains the main macros.
#include "ace/OS_main.h"

#include /**/ "ace/post.h"

#endif /* ACE_CONFIG_ALL_H */

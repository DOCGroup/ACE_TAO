
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl testSched
// ------------------------------
#ifndef TESTSCHED_EXPORT_H
#define TESTSCHED_EXPORT_H

#include "ace/config-all.h"

#if defined (TAO_AS_STATIC_LIBS)
#  if !defined (TESTSCHED_HAS_DLL)
#    define TESTSCHED_HAS_DLL 0
#  endif /* ! TESTSCHED_HAS_DLL */
#else
#  if !defined (TESTSCHED_HAS_DLL)
#    define TESTSCHED_HAS_DLL 1
#  endif /* ! TESTSCHED_HAS_DLL */
#endif

#if defined (TESTSCHED_HAS_DLL) && (TESTSCHED_HAS_DLL == 1)
#  if defined (TESTSCHED_BUILD_DLL)
#    define testSched_Export ACE_Proper_Export_Flag
#    define TESTSCHED_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define TESTSCHED_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* TESTSCHED_BUILD_DLL */
#    define testSched_Export ACE_Proper_Import_Flag
#    define TESTSCHED_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define TESTSCHED_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* TESTSCHED_BUILD_DLL */
#else /* TESTSCHED_HAS_DLL == 1 */
#  define testSched_Export
#  define TESTSCHED_SINGLETON_DECLARATION(T)
#  define TESTSCHED_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* TESTSCHED_HAS_DLL == 1 */

// Set TESTSCHED_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (TESTSCHED_NTRACE)
#  if (ACE_NTRACE == 1)
#    define TESTSCHED_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define TESTSCHED_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !TESTSCHED_NTRACE */

#if (TESTSCHED_NTRACE == 1)
#  define TESTSCHED_TRACE(X)
#else /* (TESTSCHED_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define TESTSCHED_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (TESTSCHED_NTRACE == 1) */

#endif /* TESTSCHED_EXPORT_H */

// End of auto generated file.

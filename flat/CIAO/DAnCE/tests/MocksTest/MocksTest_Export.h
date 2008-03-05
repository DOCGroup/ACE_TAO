
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl MocksTest
// ------------------------------
#ifndef MOCKSTEST_EXPORT_H
#define MOCKSTEST_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (MOCKSTEST_HAS_DLL)
#  define MOCKSTEST_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && MOCKSTEST_HAS_DLL */

#if !defined (MOCKSTEST_HAS_DLL)
#  define MOCKSTEST_HAS_DLL 1
#endif /* ! MOCKSTEST_HAS_DLL */

#if defined (MOCKSTEST_HAS_DLL) && (MOCKSTEST_HAS_DLL == 1)
#  if defined (MOCKSTEST_BUILD_DLL)
#    define MocksTest_Export ACE_Proper_Export_Flag
#    define MOCKSTEST_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define MOCKSTEST_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* MOCKSTEST_BUILD_DLL */
#    define MocksTest_Export ACE_Proper_Import_Flag
#    define MOCKSTEST_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define MOCKSTEST_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* MOCKSTEST_BUILD_DLL */
#else /* MOCKSTEST_HAS_DLL == 1 */
#  define MocksTest_Export
#  define MOCKSTEST_SINGLETON_DECLARATION(T)
#  define MOCKSTEST_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* MOCKSTEST_HAS_DLL == 1 */

// Set MOCKSTEST_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (MOCKSTEST_NTRACE)
#  if (ACE_NTRACE == 1)
#    define MOCKSTEST_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define MOCKSTEST_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !MOCKSTEST_NTRACE */

#if (MOCKSTEST_NTRACE == 1)
#  define MOCKSTEST_TRACE(X)
#else /* (MOCKSTEST_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define MOCKSTEST_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (MOCKSTEST_NTRACE == 1) */

#endif /* MOCKSTEST_EXPORT_H */

// End of auto generated file.

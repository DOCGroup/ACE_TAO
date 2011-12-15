
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl TPTEST
// ------------------------------
#ifndef TPTEST_EXPORT_H
#define TPTEST_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (TPTEST_HAS_DLL)
#  define TPTEST_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && TPTEST_HAS_DLL */

#if !defined (TPTEST_HAS_DLL)
#  define TPTEST_HAS_DLL 1
#endif /* ! TPTEST_HAS_DLL */

#if defined (TPTEST_HAS_DLL) && (TPTEST_HAS_DLL == 1)
#  if defined (TPTEST_BUILD_DLL)
#    define TPTEST_Export ACE_Proper_Export_Flag
#    define TPTEST_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define TPTEST_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* TPTEST_BUILD_DLL */
#    define TPTEST_Export ACE_Proper_Import_Flag
#    define TPTEST_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define TPTEST_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* TPTEST_BUILD_DLL */
#else /* TPTEST_HAS_DLL == 1 */
#  define TPTEST_Export
#  define TPTEST_SINGLETON_DECLARATION(T)
#  define TPTEST_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* TPTEST_HAS_DLL == 1 */

// Set TPTEST_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (TPTEST_NTRACE)
#  if (ACE_NTRACE == 1)
#    define TPTEST_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define TPTEST_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !TPTEST_NTRACE */

#if (TPTEST_NTRACE == 1)
#  define TPTEST_TRACE(X)
#else /* (TPTEST_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define TPTEST_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (TPTEST_NTRACE == 1) */

#endif /* TPTEST_EXPORT_H */

// End of auto generated file.


// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl BENCHMARK_SVNT
// ------------------------------
#ifndef BENCHMARK_SVNT_EXPORT_H
#define BENCHMARK_SVNT_EXPORT_H

#include "ace/config-all.h"

#if !defined (BENCHMARK_SVNT_HAS_DLL)
#  define BENCHMARK_SVNT_HAS_DLL 1
#endif /* ! BENCHMARK_SVNT_HAS_DLL */

#if defined (BENCHMARK_SVNT_HAS_DLL) && (BENCHMARK_SVNT_HAS_DLL == 1)
#  if defined (BENCHMARK_SVNT_BUILD_DLL)
#    define BENCHMARK_SVNT_Export ACE_Proper_Export_Flag
#    define BENCHMARK_SVNT_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define BENCHMARK_SVNT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* BENCHMARK_SVNT_BUILD_DLL */
#    define BENCHMARK_SVNT_Export ACE_Proper_Import_Flag
#    define BENCHMARK_SVNT_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define BENCHMARK_SVNT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* BENCHMARK_SVNT_BUILD_DLL */
#else /* BENCHMARK_SVNT_HAS_DLL == 1 */
#  define BENCHMARK_SVNT_Export
#  define BENCHMARK_SVNT_SINGLETON_DECLARATION(T)
#  define BENCHMARK_SVNT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* BENCHMARK_SVNT_HAS_DLL == 1 */

// Set BENCHMARK_SVNT_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (BENCHMARK_SVNT_NTRACE)
#  if (ACE_NTRACE == 1)
#    define BENCHMARK_SVNT_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define BENCHMARK_SVNT_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !BENCHMARK_SVNT_NTRACE */

#if (BENCHMARK_SVNT_NTRACE == 1)
#  define BENCHMARK_SVNT_TRACE(X)
#else /* (BENCHMARK_SVNT_NTRACE == 1) */
#  define BENCHMARK_SVNT_TRACE(X) ACE_TRACE_IMPL(X)
#endif /* (BENCHMARK_SVNT_NTRACE == 1) */

#endif /* BENCHMARK_SVNT_EXPORT_H */

// End of auto generated file.

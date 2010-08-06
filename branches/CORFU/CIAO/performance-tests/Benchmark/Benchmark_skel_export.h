
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl BENCHMARK_SVNT
// ------------------------------
#ifndef BENCHMARK_SKEL_EXPORT_H
#define BENCHMARK_SKEL_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (BENCHMARK_SKEL_HAS_DLL)
#  define BENCHMARK_SKEL_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && BENCHMARK_SKEL_HAS_DLL */

#if !defined (BENCHMARK_SKEL_HAS_DLL)
#  define BENCHMARK_SKEL_HAS_DLL 1
#endif /* ! BENCHMARK_SKEL_HAS_DLL */

#if defined (BENCHMARK_SKEL_HAS_DLL) && (BENCHMARK_SKEL_HAS_DLL == 1)
#  if defined (BENCHMARK_SKEL_BUILD_DLL)
#    define BENCHMARK_SKEL_Export ACE_Proper_Export_Flag
#    define BENCHMARK_SKEL_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define BENCHMARK_SKEL_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* BENCHMARK_SKEL_BUILD_DLL */
#    define BENCHMARK_SKEL_Export ACE_Proper_Import_Flag
#    define BENCHMARK_SKEL_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define BENCHMARK_SKEL_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* BENCHMARK_SKEL_BUILD_DLL */
#else /* BENCHMARK_SKEL_HAS_DLL == 1 */
#  define BENCHMARK_SKEL_Export
#  define BENCHMARK_SKEL_SINGLETON_DECLARATION(T)
#  define BENCHMARK_SKEL_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* BENCHMARK_SKEL_HAS_DLL == 1 */

// Set BENCHMARK_SKEL_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (BENCHMARK_SKEL_NTRACE)
#  if (ACE_NTRACE == 1)
#    define BENCHMARK_SKEL_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define BENCHMARK_SKEL_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !BENCHMARK_SKEL_NTRACE */

#if (BENCHMARK_SKEL_NTRACE == 1)
#  define BENCHMARK_SKEL_TRACE(X)
#else /* (BENCHMARK_SKEL_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define BENCHMARK_SKEL_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (BENCHMARK_SKEL_NTRACE == 1) */

#endif /* BENCHMARK_SKEL_EXPORT_H */

// End of auto generated file.

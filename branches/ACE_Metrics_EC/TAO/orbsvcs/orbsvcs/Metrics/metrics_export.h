
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl Metrics
// ------------------------------
#ifndef METRICS_EXPORT_H
#define METRICS_EXPORT_H

#include "ace/config-all.h"

#if !defined (METRICS_HAS_DLL)
#  define METRICS_HAS_DLL 1
#endif /* ! METRICS_HAS_DLL */

#if defined (METRICS_HAS_DLL) && (METRICS_HAS_DLL == 1)
#  if defined (METRICS_BUILD_DLL)
#    define Metrics_Export ACE_Proper_Export_Flag
#    define METRICS_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define METRICS_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* METRICS_BUILD_DLL */
#    define Metrics_Export ACE_Proper_Import_Flag
#    define METRICS_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define METRICS_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* METRICS_BUILD_DLL */
#else /* METRICS_HAS_DLL == 1 */
#  define Metrics_Export
#  define METRICS_SINGLETON_DECLARATION(T)
#  define METRICS_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* METRICS_HAS_DLL == 1 */

// Set METRICS_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (METRICS_NTRACE)
#  if (ACE_NTRACE == 1)
#    define METRICS_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define METRICS_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !METRICS_NTRACE */

#if (METRICS_NTRACE == 1)
#  define METRICS_TRACE(X)
#else /* (METRICS_NTRACE == 1) */
#  define METRICS_TRACE(X) ACE_TRACE_IMPL(X)
#endif /* (METRICS_NTRACE == 1) */

#endif /* METRICS_EXPORT_H */

// End of auto generated file.


// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl ANALYZER_STUB
// ------------------------------
#ifndef ANALYZER_STUB_EXPORT_H
#define ANALYZER_STUB_EXPORT_H

#include "ace/config-all.h"

#if !defined (ANALYZER_STUB_HAS_DLL)
#  define ANALYZER_STUB_HAS_DLL 1
#endif /* ! ANALYZER_STUB_HAS_DLL */

#if defined (ANALYZER_STUB_HAS_DLL) && (ANALYZER_STUB_HAS_DLL == 1)
#  if defined (ANALYZER_STUB_BUILD_DLL)
#    define ANALYZER_STUB_Export ACE_Proper_Export_Flag
#    define ANALYZER_STUB_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define ANALYZER_STUB_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* ANALYZER_STUB_BUILD_DLL */
#    define ANALYZER_STUB_Export ACE_Proper_Import_Flag
#    define ANALYZER_STUB_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define ANALYZER_STUB_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* ANALYZER_STUB_BUILD_DLL */
#else /* ANALYZER_STUB_HAS_DLL == 1 */
#  define ANALYZER_STUB_Export
#  define ANALYZER_STUB_SINGLETON_DECLARATION(T)
#  define ANALYZER_STUB_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* ANALYZER_STUB_HAS_DLL == 1 */

// Set ANALYZER_STUB_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (ANALYZER_STUB_NTRACE)
#  if (ACE_NTRACE == 1)
#    define ANALYZER_STUB_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define ANALYZER_STUB_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !ANALYZER_STUB_NTRACE */

#if (ANALYZER_STUB_NTRACE == 1)
#  define ANALYZER_STUB_TRACE(X)
#else /* (ANALYZER_STUB_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define ANALYZER_STUB_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (ANALYZER_STUB_NTRACE == 1) */

#endif /* ANALYZER_STUB_EXPORT_H */

// End of auto generated file.


// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl UCAV_STUB
// ------------------------------
#ifndef UCAV_STUB_EXPORT_H
#define UCAV_STUB_EXPORT_H

#include "ace/config-all.h"

#if !defined (UCAV_STUB_HAS_DLL)
#  define UCAV_STUB_HAS_DLL 1
#endif /* ! UCAV_STUB_HAS_DLL */

#if defined (UCAV_STUB_HAS_DLL) && (UCAV_STUB_HAS_DLL == 1)
#  if defined (UCAV_STUB_BUILD_DLL)
#    define UCAV_STUB_Export ACE_Proper_Export_Flag
#    define UCAV_STUB_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define UCAV_STUB_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* UCAV_STUB_BUILD_DLL */
#    define UCAV_STUB_Export ACE_Proper_Import_Flag
#    define UCAV_STUB_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define UCAV_STUB_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* UCAV_STUB_BUILD_DLL */
#else /* UCAV_STUB_HAS_DLL == 1 */
#  define UCAV_STUB_Export
#  define UCAV_STUB_SINGLETON_DECLARATION(T)
#  define UCAV_STUB_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* UCAV_STUB_HAS_DLL == 1 */

// Set UCAV_STUB_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (UCAV_STUB_NTRACE)
#  if (ACE_NTRACE == 1)
#    define UCAV_STUB_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define UCAV_STUB_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !UCAV_STUB_NTRACE */

#if (UCAV_STUB_NTRACE == 1)
#  define UCAV_STUB_TRACE(X)
#else /* (UCAV_STUB_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define UCAV_STUB_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (UCAV_STUB_NTRACE == 1) */

#endif /* UCAV_STUB_EXPORT_H */

// End of auto generated file.


// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl GROUND_STUB
// ------------------------------
#ifndef GROUND_STUB_EXPORT_H
#define GROUND_STUB_EXPORT_H

#include "ace/config-all.h"

#if !defined (GROUND_STUB_HAS_DLL)
#  define GROUND_STUB_HAS_DLL 1
#endif /* ! GROUND_STUB_HAS_DLL */

#if defined (GROUND_STUB_HAS_DLL) && (GROUND_STUB_HAS_DLL == 1)
#  if defined (GROUND_STUB_BUILD_DLL)
#    define GROUND_STUB_Export ACE_Proper_Export_Flag
#    define GROUND_STUB_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define GROUND_STUB_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* GROUND_STUB_BUILD_DLL */
#    define GROUND_STUB_Export ACE_Proper_Import_Flag
#    define GROUND_STUB_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define GROUND_STUB_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* GROUND_STUB_BUILD_DLL */
#else /* GROUND_STUB_HAS_DLL == 1 */
#  define GROUND_STUB_Export
#  define GROUND_STUB_SINGLETON_DECLARATION(T)
#  define GROUND_STUB_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* GROUND_STUB_HAS_DLL == 1 */

// Set GROUND_STUB_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (GROUND_STUB_NTRACE)
#  if (ACE_NTRACE == 1)
#    define GROUND_STUB_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define GROUND_STUB_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !GROUND_STUB_NTRACE */

#if (GROUND_STUB_NTRACE == 1)
#  define GROUND_STUB_TRACE(X)
#else /* (GROUND_STUB_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define GROUND_STUB_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (GROUND_STUB_NTRACE == 1) */

#endif /* GROUND_STUB_EXPORT_H */

// End of auto generated file.

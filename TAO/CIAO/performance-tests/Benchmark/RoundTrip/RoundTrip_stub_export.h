
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl ROUNDTRIP_STUB
// ------------------------------
#ifndef ROUNDTRIP_STUB_EXPORT_H
#define ROUNDTRIP_STUB_EXPORT_H

#include "ace/config-all.h"

#if !defined (ROUNDTRIP_STUB_HAS_DLL)
#  define ROUNDTRIP_STUB_HAS_DLL 1
#endif /* ! ROUNDTRIP_STUB_HAS_DLL */

#if defined (ROUNDTRIP_STUB_HAS_DLL) && (ROUNDTRIP_STUB_HAS_DLL == 1)
#  if defined (ROUNDTRIP_STUB_BUILD_DLL)
#    define ROUNDTRIP_STUB_Export ACE_Proper_Export_Flag
#    define ROUNDTRIP_STUB_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define ROUNDTRIP_STUB_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* ROUNDTRIP_STUB_BUILD_DLL */
#    define ROUNDTRIP_STUB_Export ACE_Proper_Import_Flag
#    define ROUNDTRIP_STUB_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define ROUNDTRIP_STUB_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* ROUNDTRIP_STUB_BUILD_DLL */
#else /* ROUNDTRIP_STUB_HAS_DLL == 1 */
#  define ROUNDTRIP_STUB_Export
#  define ROUNDTRIP_STUB_SINGLETON_DECLARATION(T)
#  define ROUNDTRIP_STUB_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* ROUNDTRIP_STUB_HAS_DLL == 1 */

// Set ROUNDTRIP_STUB_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (ROUNDTRIP_STUB_NTRACE)
#  if (ACE_NTRACE == 1)
#    define ROUNDTRIP_STUB_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define ROUNDTRIP_STUB_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !ROUNDTRIP_STUB_NTRACE */

#if (ROUNDTRIP_STUB_NTRACE == 1)
#  define ROUNDTRIP_STUB_TRACE(X)
#else /* (ROUNDTRIP_STUB_NTRACE == 1) */
#  define ROUNDTRIP_STUB_TRACE(X) ACE_TRACE_IMPL(X)
#endif /* (ROUNDTRIP_STUB_NTRACE == 1) */

#endif /* ROUNDTRIP_STUB_EXPORT_H */

// End of auto generated file.

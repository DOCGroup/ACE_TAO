
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl ROUNDTRIP_SVNT
// ------------------------------
#ifndef ROUNDTRIP_SVNT_EXPORT_H
#define ROUNDTRIP_SVNT_EXPORT_H

#include "ace/config-all.h"

#if !defined (ROUNDTRIP_SVNT_HAS_DLL)
#  define ROUNDTRIP_SVNT_HAS_DLL 1
#endif /* ! ROUNDTRIP_SVNT_HAS_DLL */

#if defined (ROUNDTRIP_SVNT_HAS_DLL) && (ROUNDTRIP_SVNT_HAS_DLL == 1)
#  if defined (ROUNDTRIP_SVNT_BUILD_DLL)
#    define ROUNDTRIP_SVNT_Export ACE_Proper_Export_Flag
#    define ROUNDTRIP_SVNT_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define ROUNDTRIP_SVNT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* ROUNDTRIP_SVNT_BUILD_DLL */
#    define ROUNDTRIP_SVNT_Export ACE_Proper_Import_Flag
#    define ROUNDTRIP_SVNT_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define ROUNDTRIP_SVNT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* ROUNDTRIP_SVNT_BUILD_DLL */
#else /* ROUNDTRIP_SVNT_HAS_DLL == 1 */
#  define ROUNDTRIP_SVNT_Export
#  define ROUNDTRIP_SVNT_SINGLETON_DECLARATION(T)
#  define ROUNDTRIP_SVNT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* ROUNDTRIP_SVNT_HAS_DLL == 1 */

// Set ROUNDTRIP_SVNT_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (ROUNDTRIP_SVNT_NTRACE)
#  if (ACE_NTRACE == 1)
#    define ROUNDTRIP_SVNT_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define ROUNDTRIP_SVNT_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !ROUNDTRIP_SVNT_NTRACE */

#if (ROUNDTRIP_SVNT_NTRACE == 1)
#  define ROUNDTRIP_SVNT_TRACE(X)
#else /* (ROUNDTRIP_SVNT_NTRACE == 1) */
#  define ROUNDTRIP_SVNT_TRACE(X) ACE_TRACE_IMPL(X)
#endif /* (ROUNDTRIP_SVNT_NTRACE == 1) */

#endif /* ROUNDTRIP_SVNT_EXPORT_H */

// End of auto generated file.

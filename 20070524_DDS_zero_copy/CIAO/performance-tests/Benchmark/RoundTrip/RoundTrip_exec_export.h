
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl ROUNDTRIP_EXEC
// ------------------------------
#ifndef ROUNDTRIP_EXEC_EXPORT_H
#define ROUNDTRIP_EXEC_EXPORT_H

#include "ace/config-all.h"

#if !defined (ROUNDTRIP_EXEC_HAS_DLL)
#  define ROUNDTRIP_EXEC_HAS_DLL 1
#endif /* ! ROUNDTRIP_EXEC_HAS_DLL */

#if defined (ROUNDTRIP_EXEC_HAS_DLL) && (ROUNDTRIP_EXEC_HAS_DLL == 1)
#  if defined (ROUNDTRIP_EXEC_BUILD_DLL)
#    define ROUNDTRIP_EXEC_Export ACE_Proper_Export_Flag
#    define ROUNDTRIP_EXEC_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define ROUNDTRIP_EXEC_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* ROUNDTRIP_EXEC_BUILD_DLL */
#    define ROUNDTRIP_EXEC_Export ACE_Proper_Import_Flag
#    define ROUNDTRIP_EXEC_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define ROUNDTRIP_EXEC_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* ROUNDTRIP_EXEC_BUILD_DLL */
#else /* ROUNDTRIP_EXEC_HAS_DLL == 1 */
#  define ROUNDTRIP_EXEC_Export
#  define ROUNDTRIP_EXEC_SINGLETON_DECLARATION(T)
#  define ROUNDTRIP_EXEC_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* ROUNDTRIP_EXEC_HAS_DLL == 1 */

// Set ROUNDTRIP_EXEC_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (ROUNDTRIP_EXEC_NTRACE)
#  if (ACE_NTRACE == 1)
#    define ROUNDTRIP_EXEC_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define ROUNDTRIP_EXEC_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !ROUNDTRIP_EXEC_NTRACE */

#if (ROUNDTRIP_EXEC_NTRACE == 1)
#  define ROUNDTRIP_EXEC_TRACE(X)
#else /* (ROUNDTRIP_EXEC_NTRACE == 1) */
#  define ROUNDTRIP_EXEC_TRACE(X) ACE_TRACE_IMPL(X)
#endif /* (ROUNDTRIP_EXEC_NTRACE == 1) */

#endif /* ROUNDTRIP_EXEC_EXPORT_H */

// End of auto generated file.


// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl TPCLS
// ------------------------------
#ifndef TPCLS_EXPORT_H
#define TPCLS_EXPORT_H

#include "ace/config-all.h"

#if !defined (TPCLS_HAS_DLL)
#  define TPCLS_HAS_DLL 1
#endif /* ! TPCLS_HAS_DLL */

#if defined (TPCLS_HAS_DLL) && (TPCLS_HAS_DLL == 1)
#  if defined (TPCLS_BUILD_DLL)
#    define TPCLS_Export ACE_Proper_Export_Flag
#    define TPCLS_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define TPCLS_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* TPCLS_BUILD_DLL */
#    define TPCLS_Export ACE_Proper_Import_Flag
#    define TPCLS_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define TPCLS_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* TPCLS_BUILD_DLL */
#else /* TPCLS_HAS_DLL == 1 */
#  define TPCLS_Export
#  define TPCLS_SINGLETON_DECLARATION(T)
#  define TPCLS_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* TPCLS_HAS_DLL == 1 */

// Set TPCLS_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (TPCLS_NTRACE)
#  if (ACE_NTRACE == 1)
#    define TPCLS_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define TPCLS_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !TPCLS_NTRACE */

#if (TPCLS_NTRACE == 1)
#  define TPCLS_TRACE(X)
#else /* (TPCLS_NTRACE == 1) */
#  define TPCLS_TRACE(X) ACE_TRACE_IMPL(X)
#endif /* (TPCLS_NTRACE == 1) */

#endif /* TPCLS_EXPORT_H */

// End of auto generated file.

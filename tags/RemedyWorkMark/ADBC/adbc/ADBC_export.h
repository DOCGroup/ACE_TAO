
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl ADBC
// ------------------------------
#ifndef ADBC_EXPORT_H
#define ADBC_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (ADBC_HAS_DLL)
#  define ADBC_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && ADBC_HAS_DLL */

#if !defined (ADBC_HAS_DLL)
#  define ADBC_HAS_DLL 1
#endif /* ! ADBC_HAS_DLL */

#if defined (ADBC_HAS_DLL) && (ADBC_HAS_DLL == 1)
#  if defined (ADBC_BUILD_DLL)
#    define ADBC_Export ACE_Proper_Export_Flag
#    define ADBC_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define ADBC_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* ADBC_BUILD_DLL */
#    define ADBC_Export ACE_Proper_Import_Flag
#    define ADBC_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define ADBC_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* ADBC_BUILD_DLL */
#else /* ADBC_HAS_DLL == 1 */
#  define ADBC_Export
#  define ADBC_SINGLETON_DECLARATION(T)
#  define ADBC_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* ADBC_HAS_DLL == 1 */

// Set ADBC_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (ADBC_NTRACE)
#  if (ACE_NTRACE == 1)
#    define ADBC_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define ADBC_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !ADBC_NTRACE */

#if (ADBC_NTRACE == 1)
#  define ADBC_TRACE(X)
#else /* (ADBC_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define ADBC_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (ADBC_NTRACE == 1) */

#endif /* ADBC_EXPORT_H */

// End of auto generated file.

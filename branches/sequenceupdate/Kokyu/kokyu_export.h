// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl Kokyu
// ------------------------------
#ifndef KOKYU_EXPORT_H
#define KOKYU_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS)
#  if !defined (KOKYU_HAS_DLL)
#    define KOKYU_HAS_DLL 0
#  endif /* ! KOKYU_HAS_DLL */
#else
#  if !defined (KOKYU_HAS_DLL)
#    define KOKYU_HAS_DLL 1
#  endif /* ! KOKYU_HAS_DLL */
#endif

#if defined (KOKYU_HAS_DLL) && (KOKYU_HAS_DLL == 1)
#  if defined (KOKYU_BUILD_DLL)
#    define Kokyu_Export ACE_Proper_Export_Flag
#    define KOKYU_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define KOKYU_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* KOKYU_BUILD_DLL */
#    define Kokyu_Export ACE_Proper_Import_Flag
#    define KOKYU_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define KOKYU_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* KOKYU_BUILD_DLL */
#else /* KOKYU_HAS_DLL == 1 */
#  define Kokyu_Export
#  define KOKYU_SINGLETON_DECLARATION(T)
#  define KOKYU_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* KOKYU_HAS_DLL == 1 */

// Set KOKYU_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (KOKYU_NTRACE)
#  if (ACE_NTRACE == 1)
#    define KOKYU_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define KOKYU_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !KOKYU_NTRACE */

#if (KOKYU_NTRACE == 1)
#  define KOKYU_TRACE(X)
#else /* (KOKYU_NTRACE == 1) */
#  define KOKYU_TRACE(X) ACE_TRACE_IMPL(X)
#endif /* (KOKYU_NTRACE == 1) */

#endif /* KOKYU_EXPORT_H */

// End of auto generated file.

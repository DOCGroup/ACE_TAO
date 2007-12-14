
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl HASTATUS
// ------------------------------
#ifndef HASTATUS_EXPORT_H
#define HASTATUS_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (HASTATUS_HAS_DLL)
#  define HASTATUS_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && ! HASTATUS_HAS_DLL */

#if !defined (HASTATUS_HAS_DLL)
#  define HASTATUS_HAS_DLL 1
#endif /* ! HASTATUS_HAS_DLL */

#if defined (HASTATUS_HAS_DLL) && (HASTATUS_HAS_DLL == 1)
#  if defined (HASTATUS_BUILD_DLL)
#    define HASTATUS_Export ACE_Proper_Export_Flag
#    define HASTATUS_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define HASTATUS_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* HASTATUS_BUILD_DLL */
#    define HASTATUS_Export ACE_Proper_Import_Flag
#    define HASTATUS_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define HASTATUS_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* HASTATUS_BUILD_DLL */
#else /* HASTATUS_HAS_DLL == 1 */
#  define HASTATUS_Export
#  define HASTATUS_SINGLETON_DECLARATION(T)
#  define HASTATUS_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* HASTATUS_HAS_DLL == 1 */

// Set HASTATUS_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (HASTATUS_NTRACE)
#  if (ACE_NTRACE == 1)
#    define HASTATUS_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define HASTATUS_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !HASTATUS_NTRACE */

#if (HASTATUS_NTRACE == 1)
#  define HASTATUS_TRACE(X)
#else /* (HASTATUS_NTRACE == 1) */
#  define HASTATUS_TRACE(X) ACE_TRACE_IMPL(X)
#endif /* (HASTATUS_NTRACE == 1) */

#endif /* HASTATUS_EXPORT_H */

// End of auto generated file.


// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl -s Locator
// ------------------------------
#ifndef LOCATOR_EXPORT_H
#define LOCATOR_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (LOCATOR_HAS_DLL)
#  define LOCATOR_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && LOCATOR_HAS_DLL */

#if !defined (LOCATOR_HAS_DLL)
#  define LOCATOR_HAS_DLL 1
#endif /* ! LOCATOR_HAS_DLL */

#if defined (LOCATOR_HAS_DLL) && (LOCATOR_HAS_DLL == 1)
#  if defined (LOCATOR_BUILD_DLL)
#    define Locator_Export ACE_Proper_Export_Flag
#    define LOCATOR_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define LOCATOR_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* LOCATOR_BUILD_DLL */
#    define Locator_Export ACE_Proper_Import_Flag
#    define LOCATOR_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define LOCATOR_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* LOCATOR_BUILD_DLL */
#else /* LOCATOR_HAS_DLL == 1 */
#  define Locator_Export
#  define LOCATOR_SINGLETON_DECLARATION(T)
#  define LOCATOR_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* LOCATOR_HAS_DLL == 1 */

// Set LOCATOR_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (LOCATOR_NTRACE)
#  if (ACE_NTRACE == 1)
#    define LOCATOR_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define LOCATOR_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !LOCATOR_NTRACE */

#if (LOCATOR_NTRACE == 1)
#  define LOCATOR_TRACE(X)
#else /* (LOCATOR_NTRACE == 1) */
#  define LOCATOR_TRACE(X) ACE_TRACE_IMPL(X)
#endif /* (LOCATOR_NTRACE == 1) */

#endif /* LOCATOR_EXPORT_H */

// End of auto generated file.

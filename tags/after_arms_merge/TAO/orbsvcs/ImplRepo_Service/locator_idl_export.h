
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl -s Locator
// ------------------------------
#ifndef LOCATOR_IDL_EXPORT_H
#define LOCATOR_IDL_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (LOCATOR_IDL_HAS_DLL)
#  define LOCATOR_IDL_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && LOCATOR_IDL_HAS_DLL */

#if !defined (LOCATOR_IDL_HAS_DLL)
#  define LOCATOR_IDL_HAS_DLL 1
#endif /* ! LOCATOR_IDL_HAS_DLL */

#if defined (LOCATOR_IDL_HAS_DLL) && (LOCATOR_IDL_HAS_DLL == 1)
#  if defined (LOCATOR_IDL_BUILD_DLL)
#    define LocatorIDL_Export ACE_Proper_Export_Flag
#    define LOCATOR_IDL_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define LOCATOR_IDL_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* LOCATOR_IDL_BUILD_DLL */
#    define LocatorIDL_Export ACE_Proper_Import_Flag
#    define LOCATOR_IDL_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define LOCATOR_IDL_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* LOCATOR_IDL_BUILD_DLL */
#else /* LOCATOR_IDL_HAS_DLL == 1 */
#  define LocatorIDL_Export
#  define LOCATOR_IDL_SINGLETON_DECLARATION(T)
#  define LOCATOR_IDL_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* LOCATOR_IDL_HAS_DLL == 1 */

// Set LOCATOR_IDL_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (LOCATOR_IDL_NTRACE)
#  if (ACE_NTRACE == 1)
#    define LOCATOR_IDL_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define LOCATOR_IDL_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !LOCATOR_IDL_NTRACE */

#if (LOCATOR_IDL_NTRACE == 1)
#  define LOCATOR_IDL_TRACE(X)
#else /* (LOCATOR_IDL_NTRACE == 1) */
#  define LOCATOR_IDL_TRACE(X) ACE_TRACE_IMPL(X)
#endif /* (LOCATOR_IDL_NTRACE == 1) */

#endif /* LOCATOR_IDL_EXPORT_H */

// End of auto generated file.

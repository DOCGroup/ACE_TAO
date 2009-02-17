
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl -s Based_Pointer_Export
// ------------------------------
#ifndef BASED_POINTER_EXPORT_H
#define BASED_POINTER_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (BASED_POINTER_HAS_DLL)
#  define BASED_POINTER_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && BASED_POINTER_HAS_DLL */

#if !defined (BASED_POINTER_HAS_DLL)
#  define BASED_POINTER_HAS_DLL 1
#endif /* ! BASED_POINTER_HAS_DLL */

#if defined (BASED_POINTER_HAS_DLL) && (BASED_POINTER_HAS_DLL == 1)
#  if defined (BASED_POINTER_BUILD_DLL)
#    define Based_Pointer_Export ACE_Proper_Export_Flag
#    define BASED_POINTER_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define BASED_POINTER_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* BASED_POINTER_BUILD_DLL */
#    define Based_Pointer_Export ACE_Proper_Import_Flag
#    define BASED_POINTER_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define BASED_POINTER_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* BASED_POINTER_BUILD_DLL */
#else /* BASED_POINTER_HAS_DLL == 1 */
#  define Based_Pointer_Export
#  define BASED_POINTER_SINGLETON_DECLARATION(T)
#  define BASED_POINTER_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* BASED_POINTER_HAS_DLL == 1 */

// Set BASED_POINTER_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (BASED_POINTER_NTRACE)
#  if (ACE_NTRACE == 1)
#    define BASED_POINTER_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define BASED_POINTER_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !BASED_POINTER_NTRACE */

#if (BASED_POINTER_NTRACE == 1)
#  define BASED_POINTER_TRACE(X)
#else /* (BASED_POINTER_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define BASED_POINTER_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (BASED_POINTER_NTRACE == 1) */

#endif /* BASED_POINTER_EXPORT_H */

// End of auto generated file.

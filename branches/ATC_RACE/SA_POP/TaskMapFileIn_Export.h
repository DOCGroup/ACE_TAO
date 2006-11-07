
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl TASKMAPFILEIN
// ------------------------------
#ifndef TASKMAPFILEIN_EXPORT_H
#define TASKMAPFILEIN_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (TASKMAPFILEIN_HAS_DLL)
#  define TASKMAPFILEIN_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && TASKMAPFILEIN_HAS_DLL */

#if !defined (TASKMAPFILEIN_HAS_DLL)
#  define TASKMAPFILEIN_HAS_DLL 1
#endif /* ! TASKMAPFILEIN_HAS_DLL */

#if defined (TASKMAPFILEIN_HAS_DLL) && (TASKMAPFILEIN_HAS_DLL == 1)
#  if defined (TASKMAPFILEIN_BUILD_DLL)
#    define TASKMAPFILEIN_Export ACE_Proper_Export_Flag
#    define TASKMAPFILEIN_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define TASKMAPFILEIN_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* TASKMAPFILEIN_BUILD_DLL */
#    define TASKMAPFILEIN_Export ACE_Proper_Import_Flag
#    define TASKMAPFILEIN_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define TASKMAPFILEIN_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* TASKMAPFILEIN_BUILD_DLL */
#else /* TASKMAPFILEIN_HAS_DLL == 1 */
#  define TASKMAPFILEIN_Export
#  define TASKMAPFILEIN_SINGLETON_DECLARATION(T)
#  define TASKMAPFILEIN_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* TASKMAPFILEIN_HAS_DLL == 1 */

// Set TASKMAPFILEIN_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (TASKMAPFILEIN_NTRACE)
#  if (ACE_NTRACE == 1)
#    define TASKMAPFILEIN_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define TASKMAPFILEIN_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !TASKMAPFILEIN_NTRACE */

#if (TASKMAPFILEIN_NTRACE == 1)
#  define TASKMAPFILEIN_TRACE(X)
#else /* (TASKMAPFILEIN_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define TASKMAPFILEIN_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (TASKMAPFILEIN_NTRACE == 1) */

#endif /* TASKMAPFILEIN_EXPORT_H */

// End of auto generated file.

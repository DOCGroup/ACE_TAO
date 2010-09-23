
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl Process_Priority
// ------------------------------
#ifndef PROCESS_PRIORITY_EXPORT_H
#define PROCESS_PRIORITY_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (PROCESS_PRIORITY_HAS_DLL)
#  define PROCESS_PRIORITY_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && PROCESS_PRIORITY_HAS_DLL */

#if !defined (PROCESS_PRIORITY_HAS_DLL)
#  define PROCESS_PRIORITY_HAS_DLL 1
#endif /* ! PROCESS_PRIORITY_HAS_DLL */

#if defined (PROCESS_PRIORITY_HAS_DLL) && (PROCESS_PRIORITY_HAS_DLL == 1)
#  if defined (PROCESS_PRIORITY_BUILD_DLL)
#    define Process_Priority_Export ACE_Proper_Export_Flag
#    define PROCESS_PRIORITY_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define PROCESS_PRIORITY_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* PROCESS_PRIORITY_BUILD_DLL */
#    define Process_Priority_Export ACE_Proper_Import_Flag
#    define PROCESS_PRIORITY_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define PROCESS_PRIORITY_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* PROCESS_PRIORITY_BUILD_DLL */
#else /* PROCESS_PRIORITY_HAS_DLL == 1 */
#  define Process_Priority_Export
#  define PROCESS_PRIORITY_SINGLETON_DECLARATION(T)
#  define PROCESS_PRIORITY_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* PROCESS_PRIORITY_HAS_DLL == 1 */

// Set PROCESS_PRIORITY_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (PROCESS_PRIORITY_NTRACE)
#  if (ACE_NTRACE == 1)
#    define PROCESS_PRIORITY_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define PROCESS_PRIORITY_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !PROCESS_PRIORITY_NTRACE */

#if (PROCESS_PRIORITY_NTRACE == 1)
#  define PROCESS_PRIORITY_TRACE(X)
#else /* (PROCESS_PRIORITY_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define PROCESS_PRIORITY_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (PROCESS_PRIORITY_NTRACE == 1) */

#endif /* PROCESS_PRIORITY_EXPORT_H */

// End of auto generated file.

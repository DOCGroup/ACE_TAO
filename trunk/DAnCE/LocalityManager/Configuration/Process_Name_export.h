
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl Process_Name
// ------------------------------
#ifndef PROCESS_NAME_EXPORT_H
#define PROCESS_NAME_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (PROCESS_NAME_HAS_DLL)
#  define PROCESS_NAME_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && PROCESS_NAME_HAS_DLL */

#if !defined (PROCESS_NAME_HAS_DLL)
#  define PROCESS_NAME_HAS_DLL 1
#endif /* ! PROCESS_NAME_HAS_DLL */

#if defined (PROCESS_NAME_HAS_DLL) && (PROCESS_NAME_HAS_DLL == 1)
#  if defined (PROCESS_NAME_BUILD_DLL)
#    define Process_Name_Export ACE_Proper_Export_Flag
#    define PROCESS_NAME_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define PROCESS_NAME_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* PROCESS_NAME_BUILD_DLL */
#    define Process_Name_Export ACE_Proper_Import_Flag
#    define PROCESS_NAME_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define PROCESS_NAME_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* PROCESS_NAME_BUILD_DLL */
#else /* PROCESS_NAME_HAS_DLL == 1 */
#  define Process_Name_Export
#  define PROCESS_NAME_SINGLETON_DECLARATION(T)
#  define PROCESS_NAME_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* PROCESS_NAME_HAS_DLL == 1 */

// Set PROCESS_NAME_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (PROCESS_NAME_NTRACE)
#  if (ACE_NTRACE == 1)
#    define PROCESS_NAME_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define PROCESS_NAME_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !PROCESS_NAME_NTRACE */

#if (PROCESS_NAME_NTRACE == 1)
#  define PROCESS_NAME_TRACE(X)
#else /* (PROCESS_NAME_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define PROCESS_NAME_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (PROCESS_NAME_NTRACE == 1) */

#endif /* PROCESS_NAME_EXPORT_H */

// End of auto generated file.


// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl DATA_BASE_EXEC
// ------------------------------
#ifndef DATA_BASE_EXEC_EXPORT_H
#define DATA_BASE_EXEC_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (DATA_BASE_EXEC_HAS_DLL)
#  define DATA_BASE_EXEC_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && DATA_BASE_EXEC_HAS_DLL */

#if !defined (DATA_BASE_EXEC_HAS_DLL)
#  define DATA_BASE_EXEC_HAS_DLL 1
#endif /* ! DATA_BASE_EXEC_HAS_DLL */

#if defined (DATA_BASE_EXEC_HAS_DLL) && (DATA_BASE_EXEC_HAS_DLL == 1)
#  if defined (DATA_BASE_EXEC_BUILD_DLL)
#    define DATA_BASE_EXEC_Export ACE_Proper_Export_Flag
#    define DATA_BASE_EXEC_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define DATA_BASE_EXEC_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* DATA_BASE_EXEC_BUILD_DLL */
#    define DATA_BASE_EXEC_Export ACE_Proper_Import_Flag
#    define DATA_BASE_EXEC_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define DATA_BASE_EXEC_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* DATA_BASE_EXEC_BUILD_DLL */
#else /* DATA_BASE_EXEC_HAS_DLL == 1 */
#  define DATA_BASE_EXEC_Export
#  define DATA_BASE_EXEC_SINGLETON_DECLARATION(T)
#  define DATA_BASE_EXEC_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* DATA_BASE_EXEC_HAS_DLL == 1 */

// Set DATA_BASE_EXEC_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (DATA_BASE_EXEC_NTRACE)
#  if (ACE_NTRACE == 1)
#    define DATA_BASE_EXEC_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define DATA_BASE_EXEC_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !DATA_BASE_EXEC_NTRACE */

#if (DATA_BASE_EXEC_NTRACE == 1)
#  define DATA_BASE_EXEC_TRACE(X)
#else /* (DATA_BASE_EXEC_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define DATA_BASE_EXEC_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (DATA_BASE_EXEC_NTRACE == 1) */

#endif /* DATA_BASE_EXEC_EXPORT_H */

// End of auto generated file.

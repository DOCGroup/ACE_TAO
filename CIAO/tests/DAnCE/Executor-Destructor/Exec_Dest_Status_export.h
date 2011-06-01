
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl Exec_Dest_Status
// ------------------------------
#ifndef EXEC_DEST_STATUS_EXPORT_H
#define EXEC_DEST_STATUS_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (EXEC_DEST_STATUS_HAS_DLL)
#  define EXEC_DEST_STATUS_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && EXEC_DEST_STATUS_HAS_DLL */

#if !defined (EXEC_DEST_STATUS_HAS_DLL)
#  define EXEC_DEST_STATUS_HAS_DLL 1
#endif /* ! EXEC_DEST_STATUS_HAS_DLL */

#if defined (EXEC_DEST_STATUS_HAS_DLL) && (EXEC_DEST_STATUS_HAS_DLL == 1)
#  if defined (EXEC_DEST_STATUS_BUILD_DLL)
#    define Exec_Dest_Status_Export ACE_Proper_Export_Flag
#    define EXEC_DEST_STATUS_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define EXEC_DEST_STATUS_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* EXEC_DEST_STATUS_BUILD_DLL */
#    define Exec_Dest_Status_Export ACE_Proper_Import_Flag
#    define EXEC_DEST_STATUS_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define EXEC_DEST_STATUS_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* EXEC_DEST_STATUS_BUILD_DLL */
#else /* EXEC_DEST_STATUS_HAS_DLL == 1 */
#  define Exec_Dest_Status_Export
#  define EXEC_DEST_STATUS_SINGLETON_DECLARATION(T)
#  define EXEC_DEST_STATUS_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* EXEC_DEST_STATUS_HAS_DLL == 1 */

// Set EXEC_DEST_STATUS_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (EXEC_DEST_STATUS_NTRACE)
#  if (ACE_NTRACE == 1)
#    define EXEC_DEST_STATUS_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define EXEC_DEST_STATUS_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !EXEC_DEST_STATUS_NTRACE */

#if (EXEC_DEST_STATUS_NTRACE == 1)
#  define EXEC_DEST_STATUS_TRACE(X)
#else /* (EXEC_DEST_STATUS_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define EXEC_DEST_STATUS_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (EXEC_DEST_STATUS_NTRACE == 1) */

#endif /* EXEC_DEST_STATUS_EXPORT_H */

// End of auto generated file.

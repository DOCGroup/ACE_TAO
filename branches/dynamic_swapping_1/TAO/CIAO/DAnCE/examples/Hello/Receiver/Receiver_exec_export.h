// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl RECEIVER_EXEC
// ------------------------------
#ifndef RECEIVER_EXEC_EXPORT_H
#define RECEIVER_EXEC_EXPORT_H

#include "ace/config-all.h"

#if !defined (RECEIVER_EXEC_HAS_DLL)
#  define RECEIVER_EXEC_HAS_DLL 1
#endif /* ! RECEIVER_EXEC_HAS_DLL */

#if defined (RECEIVER_EXEC_HAS_DLL) && (RECEIVER_EXEC_HAS_DLL == 1)
#  if defined (RECEIVER_EXEC_BUILD_DLL)
#    define RECEIVER_EXEC_Export ACE_Proper_Export_Flag
#    define RECEIVER_EXEC_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define RECEIVER_EXEC_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* RECEIVER_EXEC_BUILD_DLL */
#    define RECEIVER_EXEC_Export ACE_Proper_Import_Flag
#    define RECEIVER_EXEC_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define RECEIVER_EXEC_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* RECEIVER_EXEC_BUILD_DLL */
#else /* RECEIVER_EXEC_HAS_DLL == 1 */
#  define RECEIVER_EXEC_Export
#  define RECEIVER_EXEC_SINGLETON_DECLARATION(T)
#  define RECEIVER_EXEC_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* RECEIVER_EXEC_HAS_DLL == 1 */

// Set RECEIVER_EXEC_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (RECEIVER_EXEC_NTRACE)
#  if (ACE_NTRACE == 1)
#    define RECEIVER_EXEC_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define RECEIVER_EXEC_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !RECEIVER_EXEC_NTRACE */

#if (RECEIVER_EXEC_NTRACE == 1)
#  define RECEIVER_EXEC_TRACE(X)
#else /* (RECEIVER_EXEC_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define RECEIVER_EXEC_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (RECEIVER_EXEC_NTRACE == 1) */

#endif /* RECEIVER_EXEC_EXPORT_H */

// End of auto generated file.


// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl ATR_EXEC
// ------------------------------
#ifndef ATR_EXEC_EXPORT_H
#define ATR_EXEC_EXPORT_H

#include "ace/config-all.h"

#if !defined (ATR_EXEC_HAS_DLL)
#  define ATR_EXEC_HAS_DLL 1
#endif /* ! ATR_EXEC_HAS_DLL */

#if defined (ATR_EXEC_HAS_DLL) && (ATR_EXEC_HAS_DLL == 1)
#  if defined (ATR_EXEC_BUILD_DLL)
#    define ATR_EXEC_Export ACE_Proper_Export_Flag
#    define ATR_EXEC_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define ATR_EXEC_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* ATR_EXEC_BUILD_DLL */
#    define ATR_EXEC_Export ACE_Proper_Import_Flag
#    define ATR_EXEC_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define ATR_EXEC_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* ATR_EXEC_BUILD_DLL */
#else /* ATR_EXEC_HAS_DLL == 1 */
#  define ATR_EXEC_Export
#  define ATR_EXEC_SINGLETON_DECLARATION(T)
#  define ATR_EXEC_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* ATR_EXEC_HAS_DLL == 1 */

// Set ATR_EXEC_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (ATR_EXEC_NTRACE)
#  if (ACE_NTRACE == 1)
#    define ATR_EXEC_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define ATR_EXEC_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !ATR_EXEC_NTRACE */

#if (ATR_EXEC_NTRACE == 1)
#  define ATR_EXEC_TRACE(X)
#else /* (ATR_EXEC_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define ATR_EXEC_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (ATR_EXEC_NTRACE == 1) */

#endif /* ATR_EXEC_EXPORT_H */

// End of auto generated file.

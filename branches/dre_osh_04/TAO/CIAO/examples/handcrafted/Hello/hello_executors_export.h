
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl HELLO_EXECUTOR
// ------------------------------
#ifndef HELLO_EXECUTOR_EXPORT_H
#define HELLO_EXECUTOR_EXPORT_H

#include "ace/config-all.h"

#if !defined (HELLO_EXECUTOR_HAS_DLL)
#  define HELLO_EXECUTOR_HAS_DLL 1
#endif /* ! HELLO_EXECUTOR_HAS_DLL */

#if defined (HELLO_EXECUTOR_HAS_DLL) && (HELLO_EXECUTOR_HAS_DLL == 1)
#  if defined (HELLO_EXECUTOR_BUILD_DLL)
#    define HELLO_EXECUTOR_Export ACE_Proper_Export_Flag
#    define HELLO_EXECUTOR_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define HELLO_EXECUTOR_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* HELLO_EXECUTOR_BUILD_DLL */
#    define HELLO_EXECUTOR_Export ACE_Proper_Import_Flag
#    define HELLO_EXECUTOR_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define HELLO_EXECUTOR_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* HELLO_EXECUTOR_BUILD_DLL */
#else /* HELLO_EXECUTOR_HAS_DLL == 1 */
#  define HELLO_EXECUTOR_Export
#  define HELLO_EXECUTOR_SINGLETON_DECLARATION(T)
#  define HELLO_EXECUTOR_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* HELLO_EXECUTOR_HAS_DLL == 1 */

// Set HELLO_EXECUTOR_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (HELLO_EXECUTOR_NTRACE)
#  if (ACE_NTRACE == 1)
#    define HELLO_EXECUTOR_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define HELLO_EXECUTOR_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !HELLO_EXECUTOR_NTRACE */

#if (HELLO_EXECUTOR_NTRACE == 1)
#  define HELLO_EXECUTOR_TRACE(X)
#else /* (HELLO_EXECUTOR_NTRACE == 1) */
#  define HELLO_EXECUTOR_TRACE(X) ACE_TRACE_IMPL(X)
#endif /* (HELLO_EXECUTOR_NTRACE == 1) */

#endif /* HELLO_EXECUTOR_EXPORT_H */

// End of auto generated file.

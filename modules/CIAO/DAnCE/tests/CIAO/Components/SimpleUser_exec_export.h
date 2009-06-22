
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl SIMPLEUSER_EXEC
// ------------------------------
#ifndef SIMPLEUSER_EXEC_EXPORT_H
#define SIMPLEUSER_EXEC_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (SIMPLEUSER_EXEC_HAS_DLL)
#  define SIMPLEUSER_EXEC_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && SIMPLEUSER_EXEC_HAS_DLL */

#if !defined (SIMPLEUSER_EXEC_HAS_DLL)
#  define SIMPLEUSER_EXEC_HAS_DLL 1
#endif /* ! SIMPLEUSER_EXEC_HAS_DLL */

#if defined (SIMPLEUSER_EXEC_HAS_DLL) && (SIMPLEUSER_EXEC_HAS_DLL == 1)
#  if defined (SIMPLEUSER_EXEC_BUILD_DLL)
#    define SIMPLEUSER_EXEC_Export ACE_Proper_Export_Flag
#    define SIMPLEUSER_EXEC_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define SIMPLEUSER_EXEC_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* SIMPLEUSER_EXEC_BUILD_DLL */
#    define SIMPLEUSER_EXEC_Export ACE_Proper_Import_Flag
#    define SIMPLEUSER_EXEC_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define SIMPLEUSER_EXEC_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* SIMPLEUSER_EXEC_BUILD_DLL */
#else /* SIMPLEUSER_EXEC_HAS_DLL == 1 */
#  define SIMPLEUSER_EXEC_Export
#  define SIMPLEUSER_EXEC_SINGLETON_DECLARATION(T)
#  define SIMPLEUSER_EXEC_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* SIMPLEUSER_EXEC_HAS_DLL == 1 */

// Set SIMPLEUSER_EXEC_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (SIMPLEUSER_EXEC_NTRACE)
#  if (ACE_NTRACE == 1)
#    define SIMPLEUSER_EXEC_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define SIMPLEUSER_EXEC_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !SIMPLEUSER_EXEC_NTRACE */

#if (SIMPLEUSER_EXEC_NTRACE == 1)
#  define SIMPLEUSER_EXEC_TRACE(X)
#else /* (SIMPLEUSER_EXEC_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define SIMPLEUSER_EXEC_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (SIMPLEUSER_EXEC_NTRACE == 1) */

#endif /* SIMPLEUSER_EXEC_EXPORT_H */

// End of auto generated file.

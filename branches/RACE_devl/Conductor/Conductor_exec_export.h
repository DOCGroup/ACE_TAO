
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl CONDUCTOR_EXEC
// ------------------------------
#ifndef CONDUCTOR_EXEC_EXPORT_H
#define CONDUCTOR_EXEC_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (CONDUCTOR_EXEC_HAS_DLL)
#  define CONDUCTOR_EXEC_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && CONDUCTOR_EXEC_HAS_DLL */

#if !defined (CONDUCTOR_EXEC_HAS_DLL)
#  define CONDUCTOR_EXEC_HAS_DLL 1
#endif /* ! CONDUCTOR_EXEC_HAS_DLL */

#if defined (CONDUCTOR_EXEC_HAS_DLL) && (CONDUCTOR_EXEC_HAS_DLL == 1)
#  if defined (CONDUCTOR_EXEC_BUILD_DLL)
#    define CONDUCTOR_EXEC_Export ACE_Proper_Export_Flag
#    define CONDUCTOR_EXEC_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define CONDUCTOR_EXEC_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* CONDUCTOR_EXEC_BUILD_DLL */
#    define CONDUCTOR_EXEC_Export ACE_Proper_Import_Flag
#    define CONDUCTOR_EXEC_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define CONDUCTOR_EXEC_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* CONDUCTOR_EXEC_BUILD_DLL */
#else /* CONDUCTOR_EXEC_HAS_DLL == 1 */
#  define CONDUCTOR_EXEC_Export
#  define CONDUCTOR_EXEC_SINGLETON_DECLARATION(T)
#  define CONDUCTOR_EXEC_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* CONDUCTOR_EXEC_HAS_DLL == 1 */

// Set CONDUCTOR_EXEC_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (CONDUCTOR_EXEC_NTRACE)
#  if (ACE_NTRACE == 1)
#    define CONDUCTOR_EXEC_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define CONDUCTOR_EXEC_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !CONDUCTOR_EXEC_NTRACE */

#if (CONDUCTOR_EXEC_NTRACE == 1)
#  define CONDUCTOR_EXEC_TRACE(X)
#else /* (CONDUCTOR_EXEC_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define CONDUCTOR_EXEC_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (CONDUCTOR_EXEC_NTRACE == 1) */

#endif /* CONDUCTOR_EXEC_EXPORT_H */

// End of auto generated file.


// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl SATELLITE_EXEC
// ------------------------------
#ifndef SATELLITE_EXEC_EXPORT_H
#define SATELLITE_EXEC_EXPORT_H

#include "ace/config-all.h"

#if !defined (SATELLITE_EXEC_HAS_DLL)
#  define SATELLITE_EXEC_HAS_DLL 1
#endif /* ! SATELLITE_EXEC_HAS_DLL */

#if defined (SATELLITE_EXEC_HAS_DLL) && (SATELLITE_EXEC_HAS_DLL == 1)
#  if defined (SATELLITE_EXEC_BUILD_DLL)
#    define SATELLITE_EXEC_Export ACE_Proper_Export_Flag
#    define SATELLITE_EXEC_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define SATELLITE_EXEC_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* SATELLITE_EXEC_BUILD_DLL */
#    define SATELLITE_EXEC_Export ACE_Proper_Import_Flag
#    define SATELLITE_EXEC_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define SATELLITE_EXEC_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* SATELLITE_EXEC_BUILD_DLL */
#else /* SATELLITE_EXEC_HAS_DLL == 1 */
#  define SATELLITE_EXEC_Export
#  define SATELLITE_EXEC_SINGLETON_DECLARATION(T)
#  define SATELLITE_EXEC_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* SATELLITE_EXEC_HAS_DLL == 1 */

// Set SATELLITE_EXEC_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (SATELLITE_EXEC_NTRACE)
#  if (ACE_NTRACE == 1)
#    define SATELLITE_EXEC_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define SATELLITE_EXEC_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !SATELLITE_EXEC_NTRACE */

#if (SATELLITE_EXEC_NTRACE == 1)
#  define SATELLITE_EXEC_TRACE(X)
#else /* (SATELLITE_EXEC_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define SATELLITE_EXEC_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (SATELLITE_EXEC_NTRACE == 1) */

#endif /* SATELLITE_EXEC_EXPORT_H */

// End of auto generated file.

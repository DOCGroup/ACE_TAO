
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl ENGAGEMENT_EXEC
// ------------------------------
#ifndef ENGAGEMENT_EXEC_EXPORT_H
#define ENGAGEMENT_EXEC_EXPORT_H

#include "ace/config-all.h"

#if !defined (ENGAGEMENT_EXEC_HAS_DLL)
#  define ENGAGEMENT_EXEC_HAS_DLL 1
#endif /* ! ENGAGEMENT_EXEC_HAS_DLL */

#if defined (ENGAGEMENT_EXEC_HAS_DLL) && (ENGAGEMENT_EXEC_HAS_DLL == 1)
#  if defined (ENGAGEMENT_EXEC_BUILD_DLL)
#    define ENGAGEMENT_EXEC_Export ACE_Proper_Export_Flag
#    define ENGAGEMENT_EXEC_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define ENGAGEMENT_EXEC_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* ENGAGEMENT_EXEC_BUILD_DLL */
#    define ENGAGEMENT_EXEC_Export ACE_Proper_Import_Flag
#    define ENGAGEMENT_EXEC_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define ENGAGEMENT_EXEC_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* ENGAGEMENT_EXEC_BUILD_DLL */
#else /* ENGAGEMENT_EXEC_HAS_DLL == 1 */
#  define ENGAGEMENT_EXEC_Export
#  define ENGAGEMENT_EXEC_SINGLETON_DECLARATION(T)
#  define ENGAGEMENT_EXEC_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* ENGAGEMENT_EXEC_HAS_DLL == 1 */

// Set ENGAGEMENT_EXEC_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (ENGAGEMENT_EXEC_NTRACE)
#  if (ACE_NTRACE == 1)
#    define ENGAGEMENT_EXEC_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define ENGAGEMENT_EXEC_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !ENGAGEMENT_EXEC_NTRACE */

#if (ENGAGEMENT_EXEC_NTRACE == 1)
#  define ENGAGEMENT_EXEC_TRACE(X)
#else /* (ENGAGEMENT_EXEC_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define ENGAGEMENT_EXEC_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (ENGAGEMENT_EXEC_NTRACE == 1) */

#endif /* ENGAGEMENT_EXEC_EXPORT_H */

// End of auto generated file.

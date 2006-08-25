
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl RACE_COMMON_SVNT
// ------------------------------
#ifndef RACE_COMMON_SVNT_EXPORT_H
#define RACE_COMMON_SVNT_EXPORT_H

#include "ace/config-all.h"

#if !defined (RACE_COMMON_SVNT_HAS_DLL)
#  define RACE_COMMON_SVNT_HAS_DLL 1
#endif /* ! RACE_COMMON_SVNT_HAS_DLL */

#if defined (RACE_COMMON_SVNT_HAS_DLL) && (RACE_COMMON_SVNT_HAS_DLL == 1)
#  if defined (RACE_COMMON_SVNT_BUILD_DLL)
#    define RACE_COMMON_SVNT_Export ACE_Proper_Export_Flag
#    define RACE_COMMON_SVNT_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define RACE_COMMON_SVNT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* RACE_COMMON_SVNT_BUILD_DLL */
#    define RACE_COMMON_SVNT_Export ACE_Proper_Import_Flag
#    define RACE_COMMON_SVNT_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define RACE_COMMON_SVNT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* RACE_COMMON_SVNT_BUILD_DLL */
#else /* RACE_COMMON_SVNT_HAS_DLL == 1 */
#  define RACE_COMMON_SVNT_Export
#  define RACE_COMMON_SVNT_SINGLETON_DECLARATION(T)
#  define RACE_COMMON_SVNT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* RACE_COMMON_SVNT_HAS_DLL == 1 */

// Set RACE_COMMON_SVNT_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (RACE_COMMON_SVNT_NTRACE)
#  if (ACE_NTRACE == 1)
#    define RACE_COMMON_SVNT_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define RACE_COMMON_SVNT_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !RACE_COMMON_SVNT_NTRACE */

#if (RACE_COMMON_SVNT_NTRACE == 1)
#  define RACE_COMMON_SVNT_TRACE(X)
#else /* (RACE_COMMON_SVNT_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define RACE_COMMON_SVNT_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (RACE_COMMON_SVNT_NTRACE == 1) */

#endif /* RACE_COMMON_SVNT_EXPORT_H */

// End of auto generated file.

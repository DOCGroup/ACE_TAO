
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl UCAV_GS_SVNT
// ------------------------------
#ifndef UCAV_GS_SVNT_EXPORT_H
#define UCAV_GS_SVNT_EXPORT_H

#include "ace/config-all.h"

#if !defined (UCAV_GS_SVNT_HAS_DLL)
#  define UCAV_GS_SVNT_HAS_DLL 1
#endif /* ! UCAV_GS_SVNT_HAS_DLL */

#if defined (UCAV_GS_SVNT_HAS_DLL) && (UCAV_GS_SVNT_HAS_DLL == 1)
#  if defined (UCAV_GS_SVNT_BUILD_DLL)
#    define UCAV_GS_SVNT_Export ACE_Proper_Export_Flag
#    define UCAV_GS_SVNT_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define UCAV_GS_SVNT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* UCAV_GS_SVNT_BUILD_DLL */
#    define UCAV_GS_SVNT_Export ACE_Proper_Import_Flag
#    define UCAV_GS_SVNT_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define UCAV_GS_SVNT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* UCAV_GS_SVNT_BUILD_DLL */
#else /* UCAV_GS_SVNT_HAS_DLL == 1 */
#  define UCAV_GS_SVNT_Export
#  define UCAV_GS_SVNT_SINGLETON_DECLARATION(T)
#  define UCAV_GS_SVNT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* UCAV_GS_SVNT_HAS_DLL == 1 */

// Set UCAV_GS_SVNT_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (UCAV_GS_SVNT_NTRACE)
#  if (ACE_NTRACE == 1)
#    define UCAV_GS_SVNT_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define UCAV_GS_SVNT_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !UCAV_GS_SVNT_NTRACE */

#if (UCAV_GS_SVNT_NTRACE == 1)
#  define UCAV_GS_SVNT_TRACE(X)
#else /* (UCAV_GS_SVNT_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define UCAV_GS_SVNT_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (UCAV_GS_SVNT_NTRACE == 1) */

#endif /* UCAV_GS_SVNT_EXPORT_H */

// End of auto generated file.


// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl DIFFSERV_SVNT
// ------------------------------
#ifndef DIFFSERV_SVNT_EXPORT_H
#define DIFFSERV_SVNT_EXPORT_H

#include "ace/config-all.h"

#if !defined (DIFFSERV_SVNT_HAS_DLL)
#  define DIFFSERV_SVNT_HAS_DLL 1
#endif /* ! DIFFSERV_SVNT_HAS_DLL */

#if defined (DIFFSERV_SVNT_HAS_DLL) && (DIFFSERV_SVNT_HAS_DLL == 1)
#  if defined (DIFFSERV_SVNT_BUILD_DLL)
#    define DIFFSERV_SVNT_Export ACE_Proper_Export_Flag
#    define DIFFSERV_SVNT_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define DIFFSERV_SVNT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* DIFFSERV_SVNT_BUILD_DLL */
#    define DIFFSERV_SVNT_Export ACE_Proper_Import_Flag
#    define DIFFSERV_SVNT_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define DIFFSERV_SVNT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* DIFFSERV_SVNT_BUILD_DLL */
#else /* DIFFSERV_SVNT_HAS_DLL == 1 */
#  define DIFFSERV_SVNT_Export
#  define DIFFSERV_SVNT_SINGLETON_DECLARATION(T)
#  define DIFFSERV_SVNT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* DIFFSERV_SVNT_HAS_DLL == 1 */

// Set DIFFSERV_SVNT_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (DIFFSERV_SVNT_NTRACE)
#  if (ACE_NTRACE == 1)
#    define DIFFSERV_SVNT_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define DIFFSERV_SVNT_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !DIFFSERV_SVNT_NTRACE */

#if (DIFFSERV_SVNT_NTRACE == 1)
#  define DIFFSERV_SVNT_TRACE(X)
#else /* (DIFFSERV_SVNT_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define DIFFSERV_SVNT_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (DIFFSERV_SVNT_NTRACE == 1) */

#endif /* DIFFSERV_SVNT_EXPORT_H */

// End of auto generated file.

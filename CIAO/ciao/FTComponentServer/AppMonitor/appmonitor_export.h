
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl AppMonitor
// ------------------------------
#ifndef APPMONITOR_EXPORT_H
#define APPMONITOR_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (APPMONITOR_HAS_DLL)
#  define APPMONITOR_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && APPMONITOR_HAS_DLL */

#if !defined (APPMONITOR_HAS_DLL)
#  define APPMONITOR_HAS_DLL 1
#endif /* ! APPMONITOR_HAS_DLL */

#if defined (APPMONITOR_HAS_DLL) && (APPMONITOR_HAS_DLL == 1)
#  if defined (APPMONITOR_BUILD_DLL)
#    define AppMonitor_Export ACE_Proper_Export_Flag
#    define APPMONITOR_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define APPMONITOR_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* APPMONITOR_BUILD_DLL */
#    define AppMonitor_Export ACE_Proper_Import_Flag
#    define APPMONITOR_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define APPMONITOR_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* APPMONITOR_BUILD_DLL */
#else /* APPMONITOR_HAS_DLL == 1 */
#  define AppMonitor_Export
#  define APPMONITOR_SINGLETON_DECLARATION(T)
#  define APPMONITOR_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* APPMONITOR_HAS_DLL == 1 */

// Set APPMONITOR_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (APPMONITOR_NTRACE)
#  if (ACE_NTRACE == 1)
#    define APPMONITOR_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define APPMONITOR_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !APPMONITOR_NTRACE */

#if (APPMONITOR_NTRACE == 1)
#  define APPMONITOR_TRACE(X)
#else /* (APPMONITOR_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define APPMONITOR_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (APPMONITOR_NTRACE == 1) */

#endif /* APPMONITOR_EXPORT_H */

// End of auto generated file.

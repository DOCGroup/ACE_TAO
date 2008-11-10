
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl HOSTMONITOR
// ------------------------------
#ifndef HOSTMONITOR_EXPORT_H
#define HOSTMONITOR_EXPORT_H

#include "ace/config-all.h"

#if defined (TAO_AS_STATIC_LIBS)
#  if !defined (HOSTMONITOR_HAS_DLL)
#    define HOSTMONITOR_HAS_DLL 0
#  endif /* ! HOSTMONITOR_HAS_DLL */
#else
#  if !defined (HOSTMONITOR_HAS_DLL)
#    define HOSTMONITOR_HAS_DLL 1
#  endif /* ! HOSTMONITOR_HAS_DLL */
#endif

#if defined (HOSTMONITOR_HAS_DLL) && (HOSTMONITOR_HAS_DLL == 1)
#  if defined (HOSTMONITOR_BUILD_DLL)
#    define HostMonitor_Export ACE_Proper_Export_Flag
#    define HOSTMONITOR_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define HOSTMONITOR_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* HOSTMONITOR_BUILD_DLL */
#    define HostMonitor_Export ACE_Proper_Import_Flag
#    define HOSTMONITOR_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define HOSTMONITOR_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* HOSTMONITOR_BUILD_DLL */
#else /* HOSTMONITOR_HAS_DLL == 1 */
#  define HostMonitor_Export
#  define HOSTMONITOR_SINGLETON_DECLARATION(T)
#  define HOSTMONITOR_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* HOSTMONITOR_HAS_DLL == 1 */

// Set HOSTMONITOR_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (HOSTMONITOR_NTRACE)
#  if (ACE_NTRACE == 1)
#    define HOSTMONITOR_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define HOSTMONITOR_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !HOSTMONITOR_NTRACE */

#if (HOSTMONITOR_NTRACE == 1)
#  define HOSTMONITOR_TRACE(X)
#else /* (HOSTMONITOR_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define HOSTMONITOR_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (HOSTMONITOR_NTRACE == 1) */

#endif /* HOSTMONITOR_EXPORT_H */


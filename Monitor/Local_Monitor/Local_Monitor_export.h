
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl LOCAL_MONITOR
// ------------------------------
#ifndef LOCAL_MONITOR_EXPORT_H
#define LOCAL_MONITOR_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (LOCAL_MONITOR_HAS_DLL)
#  define LOCAL_MONITOR_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && LOCAL_MONITOR_HAS_DLL */

#if !defined (LOCAL_MONITOR_HAS_DLL)
#  define LOCAL_MONITOR_HAS_DLL 1
#endif /* ! LOCAL_MONITOR_HAS_DLL */

#if defined (LOCAL_MONITOR_HAS_DLL) && (LOCAL_MONITOR_HAS_DLL == 1)
#  if defined (LOCAL_MONITOR_BUILD_DLL)
#    define LOCAL_MONITOR_Export ACE_Proper_Export_Flag
#    define LOCAL_MONITOR_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define LOCAL_MONITOR_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* LOCAL_MONITOR_BUILD_DLL */
#    define LOCAL_MONITOR_Export ACE_Proper_Import_Flag
#    define LOCAL_MONITOR_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define LOCAL_MONITOR_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* LOCAL_MONITOR_BUILD_DLL */
#else /* LOCAL_MONITOR_HAS_DLL == 1 */
#  define LOCAL_MONITOR_Export
#  define LOCAL_MONITOR_SINGLETON_DECLARATION(T)
#  define LOCAL_MONITOR_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* LOCAL_MONITOR_HAS_DLL == 1 */

// Set LOCAL_MONITOR_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (LOCAL_MONITOR_NTRACE)
#  if (ACE_NTRACE == 1)
#    define LOCAL_MONITOR_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define LOCAL_MONITOR_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !LOCAL_MONITOR_NTRACE */

#if (LOCAL_MONITOR_NTRACE == 1)
#  define LOCAL_MONITOR_TRACE(X)
#else /* (LOCAL_MONITOR_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define LOCAL_MONITOR_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (LOCAL_MONITOR_NTRACE == 1) */

#endif /* LOCAL_MONITOR_EXPORT_H */

// End of auto generated file.

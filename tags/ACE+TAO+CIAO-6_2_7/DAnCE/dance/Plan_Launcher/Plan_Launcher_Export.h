
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl DAnCE_Plan_Launcher
// ------------------------------
#ifndef DANCE_PLAN_LAUNCHER_EXPORT_H
#define DANCE_PLAN_LAUNCHER_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (DANCE_PLAN_LAUNCHER_HAS_DLL)
#  define DANCE_PLAN_LAUNCHER_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && DANCE_PLAN_LAUNCHER_HAS_DLL */

#if !defined (DANCE_PLAN_LAUNCHER_HAS_DLL)
#  define DANCE_PLAN_LAUNCHER_HAS_DLL 1
#endif /* ! DANCE_PLAN_LAUNCHER_HAS_DLL */

#if defined (DANCE_PLAN_LAUNCHER_HAS_DLL) && (DANCE_PLAN_LAUNCHER_HAS_DLL == 1)
#  if defined (DANCE_PLAN_LAUNCHER_BUILD_DLL)
#    define DAnCE_Plan_Launcher_Export ACE_Proper_Export_Flag
#    define DANCE_PLAN_LAUNCHER_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define DANCE_PLAN_LAUNCHER_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* DANCE_PLAN_LAUNCHER_BUILD_DLL */
#    define DAnCE_Plan_Launcher_Export ACE_Proper_Import_Flag
#    define DANCE_PLAN_LAUNCHER_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define DANCE_PLAN_LAUNCHER_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* DANCE_PLAN_LAUNCHER_BUILD_DLL */
#else /* DANCE_PLAN_LAUNCHER_HAS_DLL == 1 */
#  define DAnCE_Plan_Launcher_Export
#  define DANCE_PLAN_LAUNCHER_SINGLETON_DECLARATION(T)
#  define DANCE_PLAN_LAUNCHER_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* DANCE_PLAN_LAUNCHER_HAS_DLL == 1 */

// Set DANCE_PLAN_LAUNCHER_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (DANCE_PLAN_LAUNCHER_NTRACE)
#  if (ACE_NTRACE == 1)
#    define DANCE_PLAN_LAUNCHER_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define DANCE_PLAN_LAUNCHER_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !DANCE_PLAN_LAUNCHER_NTRACE */

#if (DANCE_PLAN_LAUNCHER_NTRACE == 1)
#  define DANCE_PLAN_LAUNCHER_TRACE(X)
#else /* (DANCE_PLAN_LAUNCHER_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define DANCE_PLAN_LAUNCHER_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (DANCE_PLAN_LAUNCHER_NTRACE == 1) */

#endif /* DANCE_PLAN_LAUNCHER_EXPORT_H */

// End of auto generated file.


// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl EVENTS_SVNT
// ------------------------------
#ifndef EVENTS_SVNT_EXPORT_H
#define EVENTS_SVNT_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (EVENTS_SVNT_HAS_DLL)
#  define EVENTS_SVNT_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && EVENTS_SVNT_HAS_DLL */

#if !defined (EVENTS_SVNT_HAS_DLL)
#  define EVENTS_SVNT_HAS_DLL 1
#endif /* ! EVENTS_SVNT_HAS_DLL */

#if defined (EVENTS_SVNT_HAS_DLL) && (EVENTS_SVNT_HAS_DLL == 1)
#  if defined (EVENTS_SVNT_BUILD_DLL)
#    define EVENTS_SVNT_Export ACE_Proper_Export_Flag
#    define EVENTS_SVNT_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define EVENTS_SVNT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* EVENTS_SVNT_BUILD_DLL */
#    define EVENTS_SVNT_Export ACE_Proper_Import_Flag
#    define EVENTS_SVNT_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define EVENTS_SVNT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* EVENTS_SVNT_BUILD_DLL */
#else /* EVENTS_SVNT_HAS_DLL == 1 */
#  define EVENTS_SVNT_Export
#  define EVENTS_SVNT_SINGLETON_DECLARATION(T)
#  define EVENTS_SVNT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* EVENTS_SVNT_HAS_DLL == 1 */

// Set EVENTS_SVNT_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (EVENTS_SVNT_NTRACE)
#  if (ACE_NTRACE == 1)
#    define EVENTS_SVNT_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define EVENTS_SVNT_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !EVENTS_SVNT_NTRACE */

#if (EVENTS_SVNT_NTRACE == 1)
#  define EVENTS_SVNT_TRACE(X)
#else /* (EVENTS_SVNT_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define EVENTS_SVNT_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (EVENTS_SVNT_NTRACE == 1) */

#endif /* EVENTS_SVNT_EXPORT_H */

// End of auto generated file.

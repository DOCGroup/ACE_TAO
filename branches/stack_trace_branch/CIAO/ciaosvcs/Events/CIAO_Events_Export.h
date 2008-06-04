
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl CIAO_EVENTS
// ------------------------------
#ifndef CIAO_EVENTS_EXPORT_H
#define CIAO_EVENTS_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (CIAO_EVENTS_HAS_DLL)
#  define CIAO_EVENTS_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && CIAO_EVENTS_HAS_DLL */

#if !defined (CIAO_EVENTS_HAS_DLL)
#  define CIAO_EVENTS_HAS_DLL 1
#endif /* ! CIAO_EVENTS_HAS_DLL */

#if defined (CIAO_EVENTS_HAS_DLL) && (CIAO_EVENTS_HAS_DLL == 1)
#  if defined (CIAO_EVENTS_BUILD_DLL)
#    define CIAO_EVENTS_Export ACE_Proper_Export_Flag
#    define CIAO_EVENTS_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define CIAO_EVENTS_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* CIAO_EVENTS_BUILD_DLL */
#    define CIAO_EVENTS_Export ACE_Proper_Import_Flag
#    define CIAO_EVENTS_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define CIAO_EVENTS_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* CIAO_EVENTS_BUILD_DLL */
#else /* CIAO_EVENTS_HAS_DLL == 1 */
#  define CIAO_EVENTS_Export
#  define CIAO_EVENTS_SINGLETON_DECLARATION(T)
#  define CIAO_EVENTS_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* CIAO_EVENTS_HAS_DLL == 1 */

// Set CIAO_EVENTS_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (CIAO_EVENTS_NTRACE)
#  if (ACE_NTRACE == 1)
#    define CIAO_EVENTS_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define CIAO_EVENTS_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !CIAO_EVENTS_NTRACE */

#if (CIAO_EVENTS_NTRACE == 1)
#  define CIAO_EVENTS_TRACE(X)
#else /* (CIAO_EVENTS_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define CIAO_EVENTS_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (CIAO_EVENTS_NTRACE == 1) */

#endif /* CIAO_EVENTS_EXPORT_H */

// End of auto generated file.


// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl Segment_Timer
// ------------------------------
#ifndef SEGMENT_TIMER_EXPORT_H
#define SEGMENT_TIMER_EXPORT_H

#include "ace/config-all.h"

#if !defined (SEGMENT_TIMER_HAS_DLL)
#  define SEGMENT_TIMER_HAS_DLL 1
#endif /* ! SEGMENT_TIMER_HAS_DLL */

#if defined (SEGMENT_TIMER_HAS_DLL) && (SEGMENT_TIMER_HAS_DLL == 1)
#  if defined (SEGMENT_TIMER_BUILD_DLL)
#    define Segment_Timer_Export ACE_Proper_Export_Flag
#    define SEGMENT_TIMER_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define SEGMENT_TIMER_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* SEGMENT_TIMER_BUILD_DLL */
#    define Segment_Timer_Export ACE_Proper_Import_Flag
#    define SEGMENT_TIMER_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define SEGMENT_TIMER_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* SEGMENT_TIMER_BUILD_DLL */
#else /* SEGMENT_TIMER_HAS_DLL == 1 */
#  define Segment_Timer_Export
#  define SEGMENT_TIMER_SINGLETON_DECLARATION(T)
#  define SEGMENT_TIMER_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* SEGMENT_TIMER_HAS_DLL == 1 */

// Set SEGMENT_TIMER_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (SEGMENT_TIMER_NTRACE)
#  if (ACE_NTRACE == 1)
#    define SEGMENT_TIMER_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define SEGMENT_TIMER_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !SEGMENT_TIMER_NTRACE */

#if (SEGMENT_TIMER_NTRACE == 1)
#  define SEGMENT_TIMER_TRACE(X)
#else /* (SEGMENT_TIMER_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define SEGMENT_TIMER_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (SEGMENT_TIMER_NTRACE == 1) */

#endif /* SEGMENT_TIMER_EXPORT_H */

// End of auto generated file.


// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl SYNCH
// ------------------------------
#ifndef JOB_EXPORT_H
#define JOB_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (JOB_HAS_DLL)
#  define JOB_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && JOB_HAS_DLL */

#if !defined (JOB_HAS_DLL)
#  define JOB_HAS_DLL 1
#endif /* ! JOB_HAS_DLL */

#if defined (JOB_HAS_DLL) && (JOB_HAS_DLL == 1)
#  if defined (JOB_BUILD_DLL)
#    define JOB_Export ACE_Proper_Export_Flag
#    define JOB_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define JOB_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* JOB_BUILD_DLL */
#    define JOB_Export ACE_Proper_Import_Flag
#    define JOB_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define JOB_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* JOB_BUILD_DLL */
#else /* JOB_HAS_DLL == 1 */
#  define JOB_Export
#  define JOB_SINGLETON_DECLARATION(T)
#  define JOB_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* JOB_HAS_DLL == 1 */

// Set JOB_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (JOB_NTRACE)
#  if (ACE_NTRACE == 1)
#    define JOB_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define JOB_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !JOB_NTRACE */

#if (JOB_NTRACE == 1)
#  define JOB_TRACE(X)
#else /* (JOB_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define JOB_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (JOB_NTRACE == 1) */

#endif /* JOB_EXPORT_H */

// End of auto generated file.

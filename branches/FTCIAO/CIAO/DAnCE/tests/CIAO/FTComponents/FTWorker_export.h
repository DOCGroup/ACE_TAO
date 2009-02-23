
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl FTWORKER
// ------------------------------
#ifndef FTWORKER_EXPORT_H
#define FTWORKER_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (FTWORKER_HAS_DLL)
#  define FTWORKER_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && FTWORKER_HAS_DLL */

#if !defined (FTWORKER_HAS_DLL)
#  define FTWORKER_HAS_DLL 1
#endif /* ! FTWORKER_HAS_DLL */

#if defined (FTWORKER_HAS_DLL) && (FTWORKER_HAS_DLL == 1)
#  if defined (FTWORKER_BUILD_DLL)
#    define FTWORKER_Export ACE_Proper_Export_Flag
#    define FTWORKER_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define FTWORKER_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* FTWORKER_BUILD_DLL */
#    define FTWORKER_Export ACE_Proper_Import_Flag
#    define FTWORKER_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define FTWORKER_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* FTWORKER_BUILD_DLL */
#else /* FTWORKER_HAS_DLL == 1 */
#  define FTWORKER_Export
#  define FTWORKER_SINGLETON_DECLARATION(T)
#  define FTWORKER_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* FTWORKER_HAS_DLL == 1 */

// Set FTWORKER_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (FTWORKER_NTRACE)
#  if (ACE_NTRACE == 1)
#    define FTWORKER_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define FTWORKER_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !FTWORKER_NTRACE */

#if (FTWORKER_NTRACE == 1)
#  define FTWORKER_TRACE(X)
#else /* (FTWORKER_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define FTWORKER_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (FTWORKER_NTRACE == 1) */

#endif /* FTWORKER_EXPORT_H */

// End of auto generated file.

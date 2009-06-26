
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl LOGGRAPHOUT
// ------------------------------
#ifndef LOGGRAPHOUT_EXPORT_H
#define LOGGRAPHOUT_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (LOGGRAPHOUT_HAS_DLL)
#  define LOGGRAPHOUT_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && LOGGRAPHOUT_HAS_DLL */

#if !defined (LOGGRAPHOUT_HAS_DLL)
#  define LOGGRAPHOUT_HAS_DLL 1
#endif /* ! LOGGRAPHOUT_HAS_DLL */

#if defined (LOGGRAPHOUT_HAS_DLL) && (LOGGRAPHOUT_HAS_DLL == 1)
#  if defined (LOGGRAPHOUT_BUILD_DLL)
#    define LOGGRAPHOUT_Export ACE_Proper_Export_Flag
#    define LOGGRAPHOUT_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define LOGGRAPHOUT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* LOGGRAPHOUT_BUILD_DLL */
#    define LOGGRAPHOUT_Export ACE_Proper_Import_Flag
#    define LOGGRAPHOUT_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define LOGGRAPHOUT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* LOGGRAPHOUT_BUILD_DLL */
#else /* LOGGRAPHOUT_HAS_DLL == 1 */
#  define LOGGRAPHOUT_Export
#  define LOGGRAPHOUT_SINGLETON_DECLARATION(T)
#  define LOGGRAPHOUT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* LOGGRAPHOUT_HAS_DLL == 1 */

// Set LOGGRAPHOUT_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (LOGGRAPHOUT_NTRACE)
#  if (ACE_NTRACE == 1)
#    define LOGGRAPHOUT_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define LOGGRAPHOUT_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !LOGGRAPHOUT_NTRACE */

#if (LOGGRAPHOUT_NTRACE == 1)
#  define LOGGRAPHOUT_TRACE(X)
#else /* (LOGGRAPHOUT_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define LOGGRAPHOUT_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (LOGGRAPHOUT_NTRACE == 1) */

#endif /* LOGGRAPHOUT_EXPORT_H */

// End of auto generated file.

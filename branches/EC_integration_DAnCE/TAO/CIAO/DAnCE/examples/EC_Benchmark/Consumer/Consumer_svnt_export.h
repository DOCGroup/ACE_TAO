
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl CONSUMER_SVNT
// ------------------------------
#ifndef CONSUMER_SVNT_EXPORT_H
#define CONSUMER_SVNT_EXPORT_H

#include "ace/config-all.h"

#if !defined (CONSUMER_SVNT_HAS_DLL)
#  define CONSUMER_SVNT_HAS_DLL 1
#endif /* ! CONSUMER_SVNT_HAS_DLL */

#if defined (CONSUMER_SVNT_HAS_DLL) && (CONSUMER_SVNT_HAS_DLL == 1)
#  if defined (CONSUMER_SVNT_BUILD_DLL)
#    define CONSUMER_SVNT_Export ACE_Proper_Export_Flag
#    define CONSUMER_SVNT_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define CONSUMER_SVNT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* CONSUMER_SVNT_BUILD_DLL */
#    define CONSUMER_SVNT_Export ACE_Proper_Import_Flag
#    define CONSUMER_SVNT_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define CONSUMER_SVNT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* CONSUMER_SVNT_BUILD_DLL */
#else /* CONSUMER_SVNT_HAS_DLL == 1 */
#  define CONSUMER_SVNT_Export
#  define CONSUMER_SVNT_SINGLETON_DECLARATION(T)
#  define CONSUMER_SVNT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* CONSUMER_SVNT_HAS_DLL == 1 */

// Set CONSUMER_SVNT_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (CONSUMER_SVNT_NTRACE)
#  if (ACE_NTRACE == 1)
#    define CONSUMER_SVNT_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define CONSUMER_SVNT_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !CONSUMER_SVNT_NTRACE */

#if (CONSUMER_SVNT_NTRACE == 1)
#  define CONSUMER_SVNT_TRACE(X)
#else /* (CONSUMER_SVNT_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define CONSUMER_SVNT_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (CONSUMER_SVNT_NTRACE == 1) */

#endif /* CONSUMER_SVNT_EXPORT_H */

// End of auto generated file.

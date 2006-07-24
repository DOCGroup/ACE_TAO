
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl PROTOCOLS_SVNT
// ------------------------------
#ifndef PROTOCOLS_SVNT_EXPORT_H
#define PROTOCOLS_SVNT_EXPORT_H

#include "ace/config-all.h"

#if !defined (PROTOCOLS_SVNT_HAS_DLL)
#  define PROTOCOLS_SVNT_HAS_DLL 1
#endif /* ! PROTOCOLS_SVNT_HAS_DLL */

#if defined (PROTOCOLS_SVNT_HAS_DLL) && (PROTOCOLS_SVNT_HAS_DLL == 1)
#  if defined (PROTOCOLS_SVNT_BUILD_DLL)
#    define PROTOCOLS_SVNT_Export ACE_Proper_Export_Flag
#    define PROTOCOLS_SVNT_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define PROTOCOLS_SVNT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* PROTOCOLS_SVNT_BUILD_DLL */
#    define PROTOCOLS_SVNT_Export ACE_Proper_Import_Flag
#    define PROTOCOLS_SVNT_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define PROTOCOLS_SVNT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* PROTOCOLS_SVNT_BUILD_DLL */
#else /* PROTOCOLS_SVNT_HAS_DLL == 1 */
#  define PROTOCOLS_SVNT_Export
#  define PROTOCOLS_SVNT_SINGLETON_DECLARATION(T)
#  define PROTOCOLS_SVNT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* PROTOCOLS_SVNT_HAS_DLL == 1 */

// Set PROTOCOLS_SVNT_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (PROTOCOLS_SVNT_NTRACE)
#  if (ACE_NTRACE == 1)
#    define PROTOCOLS_SVNT_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define PROTOCOLS_SVNT_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !PROTOCOLS_SVNT_NTRACE */

#if (PROTOCOLS_SVNT_NTRACE == 1)
#  define PROTOCOLS_SVNT_TRACE(X)
#else /* (PROTOCOLS_SVNT_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define PROTOCOLS_SVNT_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (PROTOCOLS_SVNT_NTRACE == 1) */

#endif /* PROTOCOLS_SVNT_EXPORT_H */

// End of auto generated file.

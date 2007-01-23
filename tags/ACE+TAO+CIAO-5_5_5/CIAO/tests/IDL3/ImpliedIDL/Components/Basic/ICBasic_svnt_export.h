
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl ICBASIC_SVNT
// ------------------------------
#ifndef ICBASIC_SVNT_EXPORT_H
#define ICBASIC_SVNT_EXPORT_H

#include "ace/config-all.h"

#if !defined (ICBASIC_SVNT_HAS_DLL)
#  define ICBASIC_SVNT_HAS_DLL 1
#endif /* ! ICBASIC_SVNT_HAS_DLL */

#if defined (ICBASIC_SVNT_HAS_DLL) && (ICBASIC_SVNT_HAS_DLL == 1)
#  if defined (ICBASIC_SVNT_BUILD_DLL)
#    define ICBASIC_SVNT_Export ACE_Proper_Export_Flag
#    define ICBASIC_SVNT_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define ICBASIC_SVNT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* ICBASIC_SVNT_BUILD_DLL */
#    define ICBASIC_SVNT_Export ACE_Proper_Import_Flag
#    define ICBASIC_SVNT_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define ICBASIC_SVNT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* ICBASIC_SVNT_BUILD_DLL */
#else /* ICBASIC_SVNT_HAS_DLL == 1 */
#  define ICBASIC_SVNT_Export
#  define ICBASIC_SVNT_SINGLETON_DECLARATION(T)
#  define ICBASIC_SVNT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* ICBASIC_SVNT_HAS_DLL == 1 */

// Set ICBASIC_SVNT_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (ICBASIC_SVNT_NTRACE)
#  if (ACE_NTRACE == 1)
#    define ICBASIC_SVNT_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define ICBASIC_SVNT_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !ICBASIC_SVNT_NTRACE */

#if (ICBASIC_SVNT_NTRACE == 1)
#  define ICBASIC_SVNT_TRACE(X)
#else /* (ICBASIC_SVNT_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define ICBASIC_SVNT_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (ICBASIC_SVNT_NTRACE == 1) */

#endif /* ICBASIC_SVNT_EXPORT_H */

// End of auto generated file.

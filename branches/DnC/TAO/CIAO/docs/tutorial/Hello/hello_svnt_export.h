
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl HELLO_SVNT
// ------------------------------
#ifndef HELLO_SVNT_EXPORT_H
#define HELLO_SVNT_EXPORT_H

#include "ace/config-all.h"

#if !defined (HELLO_SVNT_HAS_DLL)
#  define HELLO_SVNT_HAS_DLL 1
#endif /* ! HELLO_SVNT_HAS_DLL */

#if defined (HELLO_SVNT_HAS_DLL) && (HELLO_SVNT_HAS_DLL == 1)
#  if defined (HELLO_SVNT_BUILD_DLL)
#    define HELLO_SVNT_Export ACE_Proper_Export_Flag
#    define HELLO_SVNT_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define HELLO_SVNT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* HELLO_SVNT_BUILD_DLL */
#    define HELLO_SVNT_Export ACE_Proper_Import_Flag
#    define HELLO_SVNT_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define HELLO_SVNT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* HELLO_SVNT_BUILD_DLL */
#else /* HELLO_SVNT_HAS_DLL == 1 */
#  define HELLO_SVNT_Export
#  define HELLO_SVNT_SINGLETON_DECLARATION(T)
#  define HELLO_SVNT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* HELLO_SVNT_HAS_DLL == 1 */

// Set HELLO_SVNT_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (HELLO_SVNT_NTRACE)
#  if (ACE_NTRACE == 1)
#    define HELLO_SVNT_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define HELLO_SVNT_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !HELLO_SVNT_NTRACE */

#if (HELLO_SVNT_NTRACE == 1)
#  define HELLO_SVNT_TRACE(X)
#else /* (HELLO_SVNT_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define HELLO_SVNT_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (HELLO_SVNT_NTRACE == 1) */

#endif /* HELLO_SVNT_EXPORT_H */

// End of auto generated file.

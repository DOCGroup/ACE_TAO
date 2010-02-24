
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl bug2926
// ------------------------------
#ifndef BUG2926_EXPORT_H
#define BUG2926_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (BUG2926_HAS_DLL)
#  define BUG2926_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && BUG2926_HAS_DLL */

#if !defined (BUG2926_HAS_DLL)
#  define BUG2926_HAS_DLL 1
#endif /* ! BUG2926_HAS_DLL */

#if defined (BUG2926_HAS_DLL) && (BUG2926_HAS_DLL == 1)
#  if defined (BUG2926_BUILD_DLL)
#    define bug2926_Export ACE_Proper_Export_Flag
#    define BUG2926_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define BUG2926_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* BUG2926_BUILD_DLL */
#    define bug2926_Export ACE_Proper_Import_Flag
#    define BUG2926_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define BUG2926_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* BUG2926_BUILD_DLL */
#else /* BUG2926_HAS_DLL == 1 */
#  define bug2926_Export
#  define BUG2926_SINGLETON_DECLARATION(T)
#  define BUG2926_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* BUG2926_HAS_DLL == 1 */

// Set BUG2926_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (BUG2926_NTRACE)
#  if (ACE_NTRACE == 1)
#    define BUG2926_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define BUG2926_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !BUG2926_NTRACE */

#if (BUG2926_NTRACE == 1)
#  define BUG2926_TRACE(X)
#else /* (BUG2926_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define BUG2926_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (BUG2926_NTRACE == 1) */

#endif /* BUG2926_EXPORT_H */

// End of auto generated file.

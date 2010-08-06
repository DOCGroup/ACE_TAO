
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl HELLO_BASE_STUB
// ------------------------------
#ifndef HELLO_BASE_STUB_EXPORT_H
#define HELLO_BASE_STUB_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (HELLO_BASE_STUB_HAS_DLL)
#  define HELLO_BASE_STUB_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && HELLO_BASE_STUB_HAS_DLL */

#if !defined (HELLO_BASE_STUB_HAS_DLL)
#  define HELLO_BASE_STUB_HAS_DLL 1
#endif /* ! HELLO_BASE_STUB_HAS_DLL */

#if defined (HELLO_BASE_STUB_HAS_DLL) && (HELLO_BASE_STUB_HAS_DLL == 1)
#  if defined (HELLO_BASE_STUB_BUILD_DLL)
#    define HELLO_BASE_STUB_Export ACE_Proper_Export_Flag
#    define HELLO_BASE_STUB_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define HELLO_BASE_STUB_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* HELLO_BASE_STUB_BUILD_DLL */
#    define HELLO_BASE_STUB_Export ACE_Proper_Import_Flag
#    define HELLO_BASE_STUB_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define HELLO_BASE_STUB_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* HELLO_BASE_STUB_BUILD_DLL */
#else /* HELLO_BASE_STUB_HAS_DLL == 1 */
#  define HELLO_BASE_STUB_Export
#  define HELLO_BASE_STUB_SINGLETON_DECLARATION(T)
#  define HELLO_BASE_STUB_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* HELLO_BASE_STUB_HAS_DLL == 1 */

// Set HELLO_BASE_STUB_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (HELLO_BASE_STUB_NTRACE)
#  if (ACE_NTRACE == 1)
#    define HELLO_BASE_STUB_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define HELLO_BASE_STUB_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !HELLO_BASE_STUB_NTRACE */

#if (HELLO_BASE_STUB_NTRACE == 1)
#  define HELLO_BASE_STUB_TRACE(X)
#else /* (HELLO_BASE_STUB_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define HELLO_BASE_STUB_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (HELLO_BASE_STUB_NTRACE == 1) */

#endif /* HELLO_BASE_STUB_EXPORT_H */

// End of auto generated file.

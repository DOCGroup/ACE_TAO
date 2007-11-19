
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl TM_PROXY_STUB
// ------------------------------
#ifndef TM_PROXY_STUB_EXPORT_H
#define TM_PROXY_STUB_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (TM_PROXY_STUB_HAS_DLL)
#  define TM_PROXY_STUB_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && TM_PROXY_STUB_HAS_DLL */

#if !defined (TM_PROXY_STUB_HAS_DLL)
#  define TM_PROXY_STUB_HAS_DLL 1
#endif /* ! TM_PROXY_STUB_HAS_DLL */

#if defined (TM_PROXY_STUB_HAS_DLL) && (TM_PROXY_STUB_HAS_DLL == 1)
#  if defined (TM_PROXY_STUB_BUILD_DLL)
#    define TM_PROXY_STUB_Export ACE_Proper_Export_Flag
#    define TM_PROXY_STUB_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define TM_PROXY_STUB_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* TM_PROXY_STUB_BUILD_DLL */
#    define TM_PROXY_STUB_Export ACE_Proper_Import_Flag
#    define TM_PROXY_STUB_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define TM_PROXY_STUB_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* TM_PROXY_STUB_BUILD_DLL */
#else /* TM_PROXY_STUB_HAS_DLL == 1 */
#  define TM_PROXY_STUB_Export
#  define TM_PROXY_STUB_SINGLETON_DECLARATION(T)
#  define TM_PROXY_STUB_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* TM_PROXY_STUB_HAS_DLL == 1 */

// Set TM_PROXY_STUB_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (TM_PROXY_STUB_NTRACE)
#  if (ACE_NTRACE == 1)
#    define TM_PROXY_STUB_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define TM_PROXY_STUB_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !TM_PROXY_STUB_NTRACE */

#if (TM_PROXY_STUB_NTRACE == 1)
#  define TM_PROXY_STUB_TRACE(X)
#else /* (TM_PROXY_STUB_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define TM_PROXY_STUB_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (TM_PROXY_STUB_NTRACE == 1) */

#endif /* TM_PROXY_STUB_EXPORT_H */

// End of auto generated file.

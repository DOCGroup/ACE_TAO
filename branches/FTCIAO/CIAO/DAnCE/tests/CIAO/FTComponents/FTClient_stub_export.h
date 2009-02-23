
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl FTCLIENT_STUB
// ------------------------------
#ifndef FTCLIENT_STUB_EXPORT_H
#define FTCLIENT_STUB_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (FTCLIENT_STUB_HAS_DLL)
#  define FTCLIENT_STUB_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && FTCLIENT_STUB_HAS_DLL */

#if !defined (FTCLIENT_STUB_HAS_DLL)
#  define FTCLIENT_STUB_HAS_DLL 1
#endif /* ! FTCLIENT_STUB_HAS_DLL */

#if defined (FTCLIENT_STUB_HAS_DLL) && (FTCLIENT_STUB_HAS_DLL == 1)
#  if defined (FTCLIENT_STUB_BUILD_DLL)
#    define FTCLIENT_STUB_Export ACE_Proper_Export_Flag
#    define FTCLIENT_STUB_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define FTCLIENT_STUB_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* FTCLIENT_STUB_BUILD_DLL */
#    define FTCLIENT_STUB_Export ACE_Proper_Import_Flag
#    define FTCLIENT_STUB_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define FTCLIENT_STUB_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* FTCLIENT_STUB_BUILD_DLL */
#else /* FTCLIENT_STUB_HAS_DLL == 1 */
#  define FTCLIENT_STUB_Export
#  define FTCLIENT_STUB_SINGLETON_DECLARATION(T)
#  define FTCLIENT_STUB_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* FTCLIENT_STUB_HAS_DLL == 1 */

// Set FTCLIENT_STUB_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (FTCLIENT_STUB_NTRACE)
#  if (ACE_NTRACE == 1)
#    define FTCLIENT_STUB_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define FTCLIENT_STUB_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !FTCLIENT_STUB_NTRACE */

#if (FTCLIENT_STUB_NTRACE == 1)
#  define FTCLIENT_STUB_TRACE(X)
#else /* (FTCLIENT_STUB_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define FTCLIENT_STUB_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (FTCLIENT_STUB_NTRACE == 1) */

#endif /* FTCLIENT_STUB_EXPORT_H */

// End of auto generated file.

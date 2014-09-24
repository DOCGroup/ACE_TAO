
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl Async_IMR_Client
// ------------------------------
#ifndef ASYNC_IMR_CLIENT_EXPORT_H
#define ASYNC_IMR_CLIENT_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (ASYNC_IMR_CLIENT_HAS_DLL)
#  define ASYNC_IMR_CLIENT_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && ASYNC_IMR_CLIENT_HAS_DLL */

#if !defined (ASYNC_IMR_CLIENT_HAS_DLL)
#  define ASYNC_IMR_CLIENT_HAS_DLL 1
#endif /* ! ASYNC_IMR_CLIENT_HAS_DLL */

#if defined (ASYNC_IMR_CLIENT_HAS_DLL) && (ASYNC_IMR_CLIENT_HAS_DLL == 1)
#  if defined (ASYNC_IMR_CLIENT_BUILD_DLL)
#    define Async_IMR_Client_Export ACE_Proper_Export_Flag
#    define ASYNC_IMR_CLIENT_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define ASYNC_IMR_CLIENT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* ASYNC_IMR_CLIENT_BUILD_DLL */
#    define Async_IMR_Client_Export ACE_Proper_Import_Flag
#    define ASYNC_IMR_CLIENT_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define ASYNC_IMR_CLIENT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* ASYNC_IMR_CLIENT_BUILD_DLL */
#else /* ASYNC_IMR_CLIENT_HAS_DLL == 1 */
#  define Async_IMR_Client_Export
#  define ASYNC_IMR_CLIENT_SINGLETON_DECLARATION(T)
#  define ASYNC_IMR_CLIENT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* ASYNC_IMR_CLIENT_HAS_DLL == 1 */

// Set ASYNC_IMR_CLIENT_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (ASYNC_IMR_CLIENT_NTRACE)
#  if (ACE_NTRACE == 1)
#    define ASYNC_IMR_CLIENT_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define ASYNC_IMR_CLIENT_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !ASYNC_IMR_CLIENT_NTRACE */

#if (ASYNC_IMR_CLIENT_NTRACE == 1)
#  define ASYNC_IMR_CLIENT_TRACE(X)
#else /* (ASYNC_IMR_CLIENT_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define ASYNC_IMR_CLIENT_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (ASYNC_IMR_CLIENT_NTRACE == 1) */

#endif /* ASYNC_IMR_CLIENT_EXPORT_H */

// End of auto generated file.

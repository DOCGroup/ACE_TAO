
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl LWFT_Client
// ------------------------------
#ifndef LWFT_CLIENT_EXPORT_H
#define LWFT_CLIENT_EXPORT_H

#include "ace/config-all.h"

#if defined (TAO_AS_STATIC_LIBS)
#  if !defined (LWFT_CLIENT_HAS_DLL)
#    define LWFT_CLIENT_HAS_DLL 0
#  endif /* ! LWFT_CLIENT_HAS_DLL */
#else
#  if !defined (LWFT_CLIENT_HAS_DLL)
#    define LWFT_CLIENT_HAS_DLL 1
#  endif /* ! LWFT_CLIENT_HAS_DLL */
#endif

#if defined (LWFT_CLIENT_HAS_DLL) && (LWFT_CLIENT_HAS_DLL == 1)
#  if defined (LWFT_CLIENT_BUILD_DLL)
#    define LWFT_Client_Export ACE_Proper_Export_Flag
#    define LWFT_CLIENT_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define LWFT_CLIENT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* LWFT_CLIENT_BUILD_DLL */
#    define LWFT_Client_Export ACE_Proper_Import_Flag
#    define LWFT_CLIENT_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define LWFT_CLIENT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* LWFT_CLIENT_BUILD_DLL */
#else /* LWFT_CLIENT_HAS_DLL == 1 */
#  define LWFT_Client_Export
#  define LWFT_CLIENT_SINGLETON_DECLARATION(T)
#  define LWFT_CLIENT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* LWFT_CLIENT_HAS_DLL == 1 */

// Set LWFT_CLIENT_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (LWFT_CLIENT_NTRACE)
#  if (ACE_NTRACE == 1)
#    define LWFT_CLIENT_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define LWFT_CLIENT_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !LWFT_CLIENT_NTRACE */

#if (LWFT_CLIENT_NTRACE == 1)
#  define LWFT_CLIENT_TRACE(X)
#else /* (LWFT_CLIENT_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define LWFT_CLIENT_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (LWFT_CLIENT_NTRACE == 1) */

#endif /* LWFT_CLIENT_EXPORT_H */


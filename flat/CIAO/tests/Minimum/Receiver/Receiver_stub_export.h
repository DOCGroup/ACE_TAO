
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl RECEIVER_STUB
// ------------------------------
#ifndef RECEIVER_STUB_EXPORT_H
#define RECEIVER_STUB_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (RECEIVER_STUB_HAS_DLL)
#  define RECEIVER_STUB_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && RECEIVER_STUB_HAS_DLL */

#if !defined (RECEIVER_STUB_HAS_DLL)
#  define RECEIVER_STUB_HAS_DLL 1
#endif /* ! RECEIVER_STUB_HAS_DLL */

#if defined (RECEIVER_STUB_HAS_DLL) && (RECEIVER_STUB_HAS_DLL == 1)
#  if defined (RECEIVER_STUB_BUILD_DLL)
#    define RECEIVER_STUB_Export ACE_Proper_Export_Flag
#    define RECEIVER_STUB_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define RECEIVER_STUB_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* RECEIVER_STUB_BUILD_DLL */
#    define RECEIVER_STUB_Export ACE_Proper_Import_Flag
#    define RECEIVER_STUB_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define RECEIVER_STUB_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* RECEIVER_STUB_BUILD_DLL */
#else /* RECEIVER_STUB_HAS_DLL == 1 */
#  define RECEIVER_STUB_Export
#  define RECEIVER_STUB_SINGLETON_DECLARATION(T)
#  define RECEIVER_STUB_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* RECEIVER_STUB_HAS_DLL == 1 */

// Set RECEIVER_STUB_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (RECEIVER_STUB_NTRACE)
#  if (ACE_NTRACE == 1)
#    define RECEIVER_STUB_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define RECEIVER_STUB_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !RECEIVER_STUB_NTRACE */

#if (RECEIVER_STUB_NTRACE == 1)
#  define RECEIVER_STUB_TRACE(X)
#else /* (RECEIVER_STUB_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define RECEIVER_STUB_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (RECEIVER_STUB_NTRACE == 1) */

#endif /* RECEIVER_STUB_EXPORT_H */

// End of auto generated file.

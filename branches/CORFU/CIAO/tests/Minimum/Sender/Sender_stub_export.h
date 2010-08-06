
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl SENDER_STUB
// ------------------------------
#ifndef SENDER_STUB_EXPORT_H
#define SENDER_STUB_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (SENDER_STUB_HAS_DLL)
#  define SENDER_STUB_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && SENDER_STUB_HAS_DLL */

#if !defined (SENDER_STUB_HAS_DLL)
#  define SENDER_STUB_HAS_DLL 1
#endif /* ! SENDER_STUB_HAS_DLL */

#if defined (SENDER_STUB_HAS_DLL) && (SENDER_STUB_HAS_DLL == 1)
#  if defined (SENDER_STUB_BUILD_DLL)
#    define SENDER_STUB_Export ACE_Proper_Export_Flag
#    define SENDER_STUB_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define SENDER_STUB_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* SENDER_STUB_BUILD_DLL */
#    define SENDER_STUB_Export ACE_Proper_Import_Flag
#    define SENDER_STUB_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define SENDER_STUB_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* SENDER_STUB_BUILD_DLL */
#else /* SENDER_STUB_HAS_DLL == 1 */
#  define SENDER_STUB_Export
#  define SENDER_STUB_SINGLETON_DECLARATION(T)
#  define SENDER_STUB_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* SENDER_STUB_HAS_DLL == 1 */

// Set SENDER_STUB_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (SENDER_STUB_NTRACE)
#  if (ACE_NTRACE == 1)
#    define SENDER_STUB_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define SENDER_STUB_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !SENDER_STUB_NTRACE */

#if (SENDER_STUB_NTRACE == 1)
#  define SENDER_STUB_TRACE(X)
#else /* (SENDER_STUB_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define SENDER_STUB_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (SENDER_STUB_NTRACE == 1) */

#endif /* SENDER_STUB_EXPORT_H */

// End of auto generated file.

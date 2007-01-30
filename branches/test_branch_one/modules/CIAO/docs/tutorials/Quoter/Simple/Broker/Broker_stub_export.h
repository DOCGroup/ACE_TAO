
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl BROKER_STUB
// ------------------------------
#ifndef BROKER_STUB_EXPORT_H
#define BROKER_STUB_EXPORT_H

#include "ace/config-all.h"

#if !defined (BROKER_STUB_HAS_DLL)
#  define BROKER_STUB_HAS_DLL 1
#endif /* ! BROKER_STUB_HAS_DLL */

#if defined (BROKER_STUB_HAS_DLL) && (BROKER_STUB_HAS_DLL == 1)
#  if defined (BROKER_STUB_BUILD_DLL)
#    define BROKER_STUB_Export ACE_Proper_Export_Flag
#    define BROKER_STUB_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define BROKER_STUB_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* BROKER_STUB_BUILD_DLL */
#    define BROKER_STUB_Export ACE_Proper_Import_Flag
#    define BROKER_STUB_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define BROKER_STUB_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* BROKER_STUB_BUILD_DLL */
#else /* BROKER_STUB_HAS_DLL == 1 */
#  define BROKER_STUB_Export
#  define BROKER_STUB_SINGLETON_DECLARATION(T)
#  define BROKER_STUB_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* BROKER_STUB_HAS_DLL == 1 */

// Set BROKER_STUB_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (BROKER_STUB_NTRACE)
#  if (ACE_NTRACE == 1)
#    define BROKER_STUB_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define BROKER_STUB_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !BROKER_STUB_NTRACE */

#if (BROKER_STUB_NTRACE == 1)
#  define BROKER_STUB_TRACE(X)
#else /* (BROKER_STUB_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define BROKER_STUB_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (BROKER_STUB_NTRACE == 1) */

#endif /* BROKER_STUB_EXPORT_H */

// End of auto generated file.


// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl NodeDaemon_stub
// ------------------------------
#ifndef NODEDAEMON_STUB_EXPORT_H
#define NODEDAEMON_STUB_EXPORT_H

#include "ace/config-all.h"

#if !defined (NODEDAEMON_STUB_HAS_DLL)
#  define NODEDAEMON_STUB_HAS_DLL 1
#endif /* ! NODEDAEMON_STUB_HAS_DLL */

#if defined (NODEDAEMON_STUB_HAS_DLL) && (NODEDAEMON_STUB_HAS_DLL == 1)
#  if defined (NODEDAEMON_STUB_BUILD_DLL)
#    define NodeDaemon_stub_Export ACE_Proper_Export_Flag
#    define NODEDAEMON_STUB_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define NODEDAEMON_STUB_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* NODEDAEMON_STUB_BUILD_DLL */
#    define NodeDaemon_stub_Export ACE_Proper_Import_Flag
#    define NODEDAEMON_STUB_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define NODEDAEMON_STUB_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* NODEDAEMON_STUB_BUILD_DLL */
#else /* NODEDAEMON_STUB_HAS_DLL == 1 */
#  define NodeDaemon_stub_Export
#  define NODEDAEMON_STUB_SINGLETON_DECLARATION(T)
#  define NODEDAEMON_STUB_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* NODEDAEMON_STUB_HAS_DLL == 1 */

// Set NODEDAEMON_STUB_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (NODEDAEMON_STUB_NTRACE)
#  if (ACE_NTRACE == 1)
#    define NODEDAEMON_STUB_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define NODEDAEMON_STUB_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !NODEDAEMON_STUB_NTRACE */

#if (NODEDAEMON_STUB_NTRACE == 1)
#  define NODEDAEMON_STUB_TRACE(X)
#else /* (NODEDAEMON_STUB_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define NODEDAEMON_STUB_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (NODEDAEMON_STUB_NTRACE == 1) */

#endif /* NODEDAEMON_STUB_EXPORT_H */

// End of auto generated file.


// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl DEPLOYMENT_STUB
// ------------------------------
#ifndef DEPLOYMENT_STUB_EXPORT_H
#define DEPLOYMENT_STUB_EXPORT_H

#include "ace/config-all.h"

#if !defined (DEPLOYMENT_STUB_HAS_DLL)
#  define DEPLOYMENT_STUB_HAS_DLL 1
#endif /* ! DEPLOYMENT_STUB_HAS_DLL */

#if defined (DEPLOYMENT_STUB_HAS_DLL) && (DEPLOYMENT_STUB_HAS_DLL == 1)
#  if defined (DEPLOYMENT_STUB_BUILD_DLL)
#    define DEPLOYMENT_STUB_Export ACE_Proper_Export_Flag
#    define DEPLOYMENT_STUB_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define DEPLOYMENT_STUB_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* DEPLOYMENT_STUB_BUILD_DLL */
#    define DEPLOYMENT_STUB_Export ACE_Proper_Import_Flag
#    define DEPLOYMENT_STUB_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define DEPLOYMENT_STUB_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* DEPLOYMENT_STUB_BUILD_DLL */
#else /* DEPLOYMENT_STUB_HAS_DLL == 1 */
#  define DEPLOYMENT_STUB_Export
#  define DEPLOYMENT_STUB_SINGLETON_DECLARATION(T)
#  define DEPLOYMENT_STUB_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* DEPLOYMENT_STUB_HAS_DLL == 1 */

// Set DEPLOYMENT_STUB_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (DEPLOYMENT_STUB_NTRACE)
#  if (ACE_NTRACE == 1)
#    define DEPLOYMENT_STUB_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define DEPLOYMENT_STUB_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !DEPLOYMENT_STUB_NTRACE */

#if (DEPLOYMENT_STUB_NTRACE == 1)
#  define DEPLOYMENT_STUB_TRACE(X)
#else /* (DEPLOYMENT_STUB_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define DEPLOYMENT_STUB_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (DEPLOYMENT_STUB_NTRACE == 1) */

#endif /* DEPLOYMENT_STUB_EXPORT_H */

// End of auto generated file.

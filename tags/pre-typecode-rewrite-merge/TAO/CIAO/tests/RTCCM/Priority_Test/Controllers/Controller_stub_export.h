
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl CONTROLLER_STUB
// ------------------------------
#ifndef CONTROLLER_STUB_EXPORT_H
#define CONTROLLER_STUB_EXPORT_H

#include "ace/config-all.h"

#if !defined (CONTROLLER_STUB_HAS_DLL)
#  define CONTROLLER_STUB_HAS_DLL 1
#endif /* ! CONTROLLER_STUB_HAS_DLL */

#if defined (CONTROLLER_STUB_HAS_DLL) && (CONTROLLER_STUB_HAS_DLL == 1)
#  if defined (CONTROLLER_STUB_BUILD_DLL)
#    define CONTROLLER_STUB_Export ACE_Proper_Export_Flag
#    define CONTROLLER_STUB_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define CONTROLLER_STUB_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* CONTROLLER_STUB_BUILD_DLL */
#    define CONTROLLER_STUB_Export ACE_Proper_Import_Flag
#    define CONTROLLER_STUB_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define CONTROLLER_STUB_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* CONTROLLER_STUB_BUILD_DLL */
#else /* CONTROLLER_STUB_HAS_DLL == 1 */
#  define CONTROLLER_STUB_Export
#  define CONTROLLER_STUB_SINGLETON_DECLARATION(T)
#  define CONTROLLER_STUB_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* CONTROLLER_STUB_HAS_DLL == 1 */

// Set CONTROLLER_STUB_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (CONTROLLER_STUB_NTRACE)
#  if (ACE_NTRACE == 1)
#    define CONTROLLER_STUB_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define CONTROLLER_STUB_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !CONTROLLER_STUB_NTRACE */

#if (CONTROLLER_STUB_NTRACE == 1)
#  define CONTROLLER_STUB_TRACE(X)
#else /* (CONTROLLER_STUB_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define CONTROLLER_STUB_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (CONTROLLER_STUB_NTRACE == 1) */

#endif /* CONTROLLER_STUB_EXPORT_H */

// End of auto generated file.

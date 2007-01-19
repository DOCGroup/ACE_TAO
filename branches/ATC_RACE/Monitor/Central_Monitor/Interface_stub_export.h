
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl INTERFACE_STUB
// ------------------------------
#ifndef INTERFACE_STUB_EXPORT_H
#define INTERFACE_STUB_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (INTERFACE_STUB_HAS_DLL)
#  define INTERFACE_STUB_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && INTERFACE_STUB_HAS_DLL */

#if !defined (INTERFACE_STUB_HAS_DLL)
#  define INTERFACE_STUB_HAS_DLL 1
#endif /* ! INTERFACE_STUB_HAS_DLL */

#if defined (INTERFACE_STUB_HAS_DLL) && (INTERFACE_STUB_HAS_DLL == 1)
#  if defined (INTERFACE_STUB_BUILD_DLL)
#    define INTERFACE_STUB_Export ACE_Proper_Export_Flag
#    define INTERFACE_STUB_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define INTERFACE_STUB_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* INTERFACE_STUB_BUILD_DLL */
#    define INTERFACE_STUB_Export ACE_Proper_Import_Flag
#    define INTERFACE_STUB_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define INTERFACE_STUB_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* INTERFACE_STUB_BUILD_DLL */
#else /* INTERFACE_STUB_HAS_DLL == 1 */
#  define INTERFACE_STUB_Export
#  define INTERFACE_STUB_SINGLETON_DECLARATION(T)
#  define INTERFACE_STUB_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* INTERFACE_STUB_HAS_DLL == 1 */

// Set INTERFACE_STUB_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (INTERFACE_STUB_NTRACE)
#  if (ACE_NTRACE == 1)
#    define INTERFACE_STUB_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define INTERFACE_STUB_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !INTERFACE_STUB_NTRACE */

#if (INTERFACE_STUB_NTRACE == 1)
#  define INTERFACE_STUB_TRACE(X)
#else /* (INTERFACE_STUB_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define INTERFACE_STUB_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (INTERFACE_STUB_NTRACE == 1) */

#endif /* INTERFACE_STUB_EXPORT_H */

// End of auto generated file.

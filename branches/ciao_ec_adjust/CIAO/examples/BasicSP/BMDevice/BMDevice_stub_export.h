
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl -s BMDEVICE_STUB
// ------------------------------
#ifndef BMDEVICE_STUB_EXPORT_H
#define BMDEVICE_STUB_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (BMDEVICE_STUB_HAS_DLL)
#  define BMDEVICE_STUB_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && BMDEVICE_STUB_HAS_DLL */

#if !defined (BMDEVICE_STUB_HAS_DLL)
#  define BMDEVICE_STUB_HAS_DLL 1
#endif /* ! BMDEVICE_STUB_HAS_DLL */

#if defined (BMDEVICE_STUB_HAS_DLL) && (BMDEVICE_STUB_HAS_DLL == 1)
#  if defined (BMDEVICE_STUB_BUILD_DLL)
#    define BMDEVICE_STUB_Export ACE_Proper_Export_Flag
#    define BMDEVICE_STUB_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define BMDEVICE_STUB_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* BMDEVICE_STUB_BUILD_DLL */
#    define BMDEVICE_STUB_Export ACE_Proper_Import_Flag
#    define BMDEVICE_STUB_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define BMDEVICE_STUB_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* BMDEVICE_STUB_BUILD_DLL */
#else /* BMDEVICE_STUB_HAS_DLL == 1 */
#  define BMDEVICE_STUB_Export
#  define BMDEVICE_STUB_SINGLETON_DECLARATION(T)
#  define BMDEVICE_STUB_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* BMDEVICE_STUB_HAS_DLL == 1 */

// Set BMDEVICE_STUB_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (BMDEVICE_STUB_NTRACE)
#  if (ACE_NTRACE == 1)
#    define BMDEVICE_STUB_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define BMDEVICE_STUB_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !BMDEVICE_STUB_NTRACE */

#if (BMDEVICE_STUB_NTRACE == 1)
#  define BMDEVICE_STUB_TRACE(X)
#else /* (BMDEVICE_STUB_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define BMDEVICE_STUB_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (BMDEVICE_STUB_NTRACE == 1) */

#endif /* BMDEVICE_STUB_EXPORT_H */

// End of auto generated file.

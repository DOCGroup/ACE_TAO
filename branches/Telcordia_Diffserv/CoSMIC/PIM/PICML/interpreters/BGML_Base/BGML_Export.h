
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl BGML
// ------------------------------
#ifndef BGML_EXPORT_H
#define BGML_EXPORT_H

#include "ace/config-all.h"

#if !defined (BGML_HAS_DLL)
#  define BGML_HAS_DLL 1
#endif /* ! BGML_HAS_DLL */

#if defined (BGML_HAS_DLL) && (BGML_HAS_DLL == 1)
#  if defined (BGML_BUILD_DLL)
#    define BGML_Export ACE_Proper_Export_Flag
#    define BGML_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define BGML_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* BGML_BUILD_DLL */
#    define BGML_Export ACE_Proper_Import_Flag
#    define BGML_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define BGML_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* BGML_BUILD_DLL */
#else /* BGML_HAS_DLL == 1 */
#  define BGML_Export
#  define BGML_SINGLETON_DECLARATION(T)
#  define BGML_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* BGML_HAS_DLL == 1 */

// Set BGML_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (BGML_NTRACE)
#  if (ACE_NTRACE == 1)
#    define BGML_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define BGML_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !BGML_NTRACE */

#if (BGML_NTRACE == 1)
#  define BGML_TRACE(X)
#else /* (BGML_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define BGML_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (BGML_NTRACE == 1) */

#endif /* BGML_EXPORT_H */

// End of auto generated file.

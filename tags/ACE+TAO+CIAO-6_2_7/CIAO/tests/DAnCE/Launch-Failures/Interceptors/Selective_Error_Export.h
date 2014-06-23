
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl Selective_Error
// ------------------------------
#ifndef SELECTIVE_ERROR_EXPORT_H
#define SELECTIVE_ERROR_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (SELECTIVE_ERROR_HAS_DLL)
#  define SELECTIVE_ERROR_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && SELECTIVE_ERROR_HAS_DLL */

#if !defined (SELECTIVE_ERROR_HAS_DLL)
#  define SELECTIVE_ERROR_HAS_DLL 1
#endif /* ! SELECTIVE_ERROR_HAS_DLL */

#if defined (SELECTIVE_ERROR_HAS_DLL) && (SELECTIVE_ERROR_HAS_DLL == 1)
#  if defined (SELECTIVE_ERROR_BUILD_DLL)
#    define Selective_Error_Export ACE_Proper_Export_Flag
#    define SELECTIVE_ERROR_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define SELECTIVE_ERROR_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* SELECTIVE_ERROR_BUILD_DLL */
#    define Selective_Error_Export ACE_Proper_Import_Flag
#    define SELECTIVE_ERROR_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define SELECTIVE_ERROR_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* SELECTIVE_ERROR_BUILD_DLL */
#else /* SELECTIVE_ERROR_HAS_DLL == 1 */
#  define Selective_Error_Export
#  define SELECTIVE_ERROR_SINGLETON_DECLARATION(T)
#  define SELECTIVE_ERROR_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* SELECTIVE_ERROR_HAS_DLL == 1 */

// Set SELECTIVE_ERROR_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (SELECTIVE_ERROR_NTRACE)
#  if (ACE_NTRACE == 1)
#    define SELECTIVE_ERROR_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define SELECTIVE_ERROR_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !SELECTIVE_ERROR_NTRACE */

#if (SELECTIVE_ERROR_NTRACE == 1)
#  define SELECTIVE_ERROR_TRACE(X)
#else /* (SELECTIVE_ERROR_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define SELECTIVE_ERROR_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (SELECTIVE_ERROR_NTRACE == 1) */

#endif /* SELECTIVE_ERROR_EXPORT_H */

// End of auto generated file.

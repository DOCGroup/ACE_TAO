
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl Context_Impl
// ------------------------------
#ifndef CONTEXT_IMPL_EXPORT_H
#define CONTEXT_IMPL_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (CONTEXT_IMPL_HAS_DLL)
#  define CONTEXT_IMPL_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && CONTEXT_IMPL_HAS_DLL */

#if !defined (CONTEXT_IMPL_HAS_DLL)
#  define CONTEXT_IMPL_HAS_DLL 1
#endif /* ! CONTEXT_IMPL_HAS_DLL */

#if defined (CONTEXT_IMPL_HAS_DLL) && (CONTEXT_IMPL_HAS_DLL == 1)
#  if defined (CONTEXT_IMPL_BUILD_DLL)
#    define Context_Impl_Export ACE_Proper_Export_Flag
#    define CONTEXT_IMPL_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define CONTEXT_IMPL_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* CONTEXT_IMPL_BUILD_DLL */
#    define Context_Impl_Export ACE_Proper_Import_Flag
#    define CONTEXT_IMPL_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define CONTEXT_IMPL_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* CONTEXT_IMPL_BUILD_DLL */
#else /* CONTEXT_IMPL_HAS_DLL == 1 */
#  define Context_Impl_Export
#  define CONTEXT_IMPL_SINGLETON_DECLARATION(T)
#  define CONTEXT_IMPL_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* CONTEXT_IMPL_HAS_DLL == 1 */

// Set CONTEXT_IMPL_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (CONTEXT_IMPL_NTRACE)
#  if (ACE_NTRACE == 1)
#    define CONTEXT_IMPL_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define CONTEXT_IMPL_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !CONTEXT_IMPL_NTRACE */

#if (CONTEXT_IMPL_NTRACE == 1)
#  define CONTEXT_IMPL_TRACE(X)
#else /* (CONTEXT_IMPL_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define CONTEXT_IMPL_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (CONTEXT_IMPL_NTRACE == 1) */

#endif /* CONTEXT_IMPL_EXPORT_H */

// End of auto generated file.

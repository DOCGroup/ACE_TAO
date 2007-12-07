
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl DDSTopic
// ------------------------------
#ifndef DDSTOPIC_EXPORT_H
#define DDSTOPIC_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (DDSTOPIC_HAS_DLL)
#  define DDSTOPIC_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && DDSTOPIC_HAS_DLL */

#if !defined (DDSTOPIC_HAS_DLL)
#  define DDSTOPIC_HAS_DLL 1
#endif /* ! DDSTOPIC_HAS_DLL */

#if defined (DDSTOPIC_HAS_DLL) && (DDSTOPIC_HAS_DLL == 1)
#  if defined (DDSTOPIC_BUILD_DLL)
#    define DDSTopic_Export ACE_Proper_Export_Flag
#    define DDSTOPIC_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define DDSTOPIC_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* DDSTOPIC_BUILD_DLL */
#    define DDSTopic_Export ACE_Proper_Import_Flag
#    define DDSTOPIC_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define DDSTOPIC_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* DDSTOPIC_BUILD_DLL */
#else /* DDSTOPIC_HAS_DLL == 1 */
#  define DDSTopic_Export
#  define DDSTOPIC_SINGLETON_DECLARATION(T)
#  define DDSTOPIC_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* DDSTOPIC_HAS_DLL == 1 */

// Set DDSTOPIC_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (DDSTOPIC_NTRACE)
#  if (ACE_NTRACE == 1)
#    define DDSTOPIC_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define DDSTOPIC_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !DDSTOPIC_NTRACE */

#if (DDSTOPIC_NTRACE == 1)
#  define DDSTOPIC_TRACE(X)
#else /* (DDSTOPIC_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define DDSTOPIC_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (DDSTOPIC_NTRACE == 1) */

#endif /* DDSTOPIC_EXPORT_H */

// End of auto generated file.

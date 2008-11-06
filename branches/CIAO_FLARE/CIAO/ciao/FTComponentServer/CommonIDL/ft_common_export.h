
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl FT_Common
// ------------------------------
#ifndef FT_COMMON_EXPORT_H
#define FT_COMMON_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (FT_COMMON_HAS_DLL)
#  define FT_COMMON_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && FT_COMMON_HAS_DLL */

#if !defined (FT_COMMON_HAS_DLL)
#  define FT_COMMON_HAS_DLL 1
#endif /* ! FT_COMMON_HAS_DLL */

#if defined (FT_COMMON_HAS_DLL) && (FT_COMMON_HAS_DLL == 1)
#  if defined (FT_COMMON_BUILD_DLL)
#    define FT_Common_Export ACE_Proper_Export_Flag
#    define FT_COMMON_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define FT_COMMON_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* FT_COMMON_BUILD_DLL */
#    define FT_Common_Export ACE_Proper_Import_Flag
#    define FT_COMMON_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define FT_COMMON_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* FT_COMMON_BUILD_DLL */
#else /* FT_COMMON_HAS_DLL == 1 */
#  define FT_Common_Export
#  define FT_COMMON_SINGLETON_DECLARATION(T)
#  define FT_COMMON_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* FT_COMMON_HAS_DLL == 1 */

// Set FT_COMMON_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (FT_COMMON_NTRACE)
#  if (ACE_NTRACE == 1)
#    define FT_COMMON_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define FT_COMMON_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !FT_COMMON_NTRACE */

#if (FT_COMMON_NTRACE == 1)
#  define FT_COMMON_TRACE(X)
#else /* (FT_COMMON_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define FT_COMMON_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (FT_COMMON_NTRACE == 1) */

#endif /* FT_COMMON_EXPORT_H */

// End of auto generated file.

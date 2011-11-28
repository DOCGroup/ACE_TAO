
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl FP_DT_Creator
// ------------------------------
#ifndef FP_DT_CREATOR_EXPORT_H
#define FP_DT_CREATOR_EXPORT_H

#include "ace/config-all.h"

#if !defined (FP_DT_CREATOR_HAS_DLL)
#  define FP_DT_CREATOR_HAS_DLL 1
#endif /* ! FP_DT_CREATOR_HAS_DLL */

#if defined (FP_DT_CREATOR_HAS_DLL) && (FP_DT_CREATOR_HAS_DLL == 1)
#  if defined (FP_DT_CREATOR_BUILD_DLL)
#    define FP_DT_Creator_Export ACE_Proper_Export_Flag
#    define FP_DT_CREATOR_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define FP_DT_CREATOR_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* FP_DT_CREATOR_BUILD_DLL */
#    define FP_DT_Creator_Export ACE_Proper_Import_Flag
#    define FP_DT_CREATOR_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define FP_DT_CREATOR_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* FP_DT_CREATOR_BUILD_DLL */
#else /* FP_DT_CREATOR_HAS_DLL == 1 */
#  define FP_DT_Creator_Export
#  define FP_DT_CREATOR_SINGLETON_DECLARATION(T)
#  define FP_DT_CREATOR_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* FP_DT_CREATOR_HAS_DLL == 1 */

// Set FP_DT_CREATOR_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (FP_DT_CREATOR_NTRACE)
#  if (ACE_NTRACE == 1)
#    define FP_DT_CREATOR_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define FP_DT_CREATOR_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !FP_DT_CREATOR_NTRACE */

#if (FP_DT_CREATOR_NTRACE == 1)
#  define FP_DT_CREATOR_TRACE(X)
#else /* (FP_DT_CREATOR_NTRACE == 1) */
#  define FP_DT_CREATOR_TRACE(X) ACE_TRACE_IMPL(X)
#endif /* (FP_DT_CREATOR_NTRACE == 1) */

#endif /* FP_DT_CREATOR_EXPORT_H */

// End of auto generated file.


// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl ENW_SVNT
// ------------------------------
#ifndef ENW_SVNT_EXPORT_H
#define ENW_SVNT_EXPORT_H

#include "ace/config-all.h"

#if !defined (ENW_SVNT_HAS_DLL)
#  define ENW_SVNT_HAS_DLL 1
#endif /* ! ENW_SVNT_HAS_DLL */

#if defined (ENW_SVNT_HAS_DLL) && (ENW_SVNT_HAS_DLL == 1)
#  if defined (ENW_SVNT_BUILD_DLL)
#    define ENW_SVNT_Export ACE_Proper_Export_Flag
#    define ENW_SVNT_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define ENW_SVNT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* ENW_SVNT_BUILD_DLL */
#    define ENW_SVNT_Export ACE_Proper_Import_Flag
#    define ENW_SVNT_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define ENW_SVNT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* ENW_SVNT_BUILD_DLL */
#else /* ENW_SVNT_HAS_DLL == 1 */
#  define ENW_SVNT_Export
#  define ENW_SVNT_SINGLETON_DECLARATION(T)
#  define ENW_SVNT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* ENW_SVNT_HAS_DLL == 1 */

// Set ENW_SVNT_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (ENW_SVNT_NTRACE)
#  if (ACE_NTRACE == 1)
#    define ENW_SVNT_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define ENW_SVNT_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !ENW_SVNT_NTRACE */

#if (ENW_SVNT_NTRACE == 1)
#  define ENW_SVNT_TRACE(X)
#else /* (ENW_SVNT_NTRACE == 1) */
#  define ENW_SVNT_TRACE(X) ACE_TRACE_IMPL(X)
#endif /* (ENW_SVNT_NTRACE == 1) */

#endif /* ENW_SVNT_EXPORT_H */

// End of auto generated file.

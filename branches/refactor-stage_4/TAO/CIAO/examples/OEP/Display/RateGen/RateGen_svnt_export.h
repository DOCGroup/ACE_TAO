
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl RATEGEN_SVNT
// ------------------------------
#ifndef RATEGEN_SVNT_EXPORT_H
#define RATEGEN_SVNT_EXPORT_H

#include "ace/config-all.h"

#if !defined (RATEGEN_SVNT_HAS_DLL)
#  define RATEGEN_SVNT_HAS_DLL 1
#endif /* ! RATEGEN_SVNT_HAS_DLL */

#if defined (RATEGEN_SVNT_HAS_DLL) && (RATEGEN_SVNT_HAS_DLL == 1)
#  if defined (RATEGEN_SVNT_BUILD_DLL)
#    define RATEGEN_SVNT_Export ACE_Proper_Export_Flag
#    define RATEGEN_SVNT_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define RATEGEN_SVNT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* RATEGEN_SVNT_BUILD_DLL */
#    define RATEGEN_SVNT_Export ACE_Proper_Import_Flag
#    define RATEGEN_SVNT_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define RATEGEN_SVNT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* RATEGEN_SVNT_BUILD_DLL */
#else /* RATEGEN_SVNT_HAS_DLL == 1 */
#  define RATEGEN_SVNT_Export
#  define RATEGEN_SVNT_SINGLETON_DECLARATION(T)
#  define RATEGEN_SVNT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* RATEGEN_SVNT_HAS_DLL == 1 */

// Set RATEGEN_SVNT_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (RATEGEN_SVNT_NTRACE)
#  if (ACE_NTRACE == 1)
#    define RATEGEN_SVNT_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define RATEGEN_SVNT_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !RATEGEN_SVNT_NTRACE */

#if (RATEGEN_SVNT_NTRACE == 1)
#  define RATEGEN_SVNT_TRACE(X)
#else /* (RATEGEN_SVNT_NTRACE == 1) */
#  define RATEGEN_SVNT_TRACE(X) ACE_TRACE_IMPL(X)
#endif /* (RATEGEN_SVNT_NTRACE == 1) */

#endif /* RATEGEN_SVNT_EXPORT_H */

// End of auto generated file.


// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl AC_CLD
// ------------------------------
#ifndef AC_CLD_EXPORT_H
#define AC_CLD_EXPORT_H

#include "ace/config-all.h"

#if !defined (AC_CLD_HAS_DLL)
#  define AC_CLD_HAS_DLL 1
#endif /* ! AC_CLD_HAS_DLL */

#if defined (AC_CLD_HAS_DLL) && (AC_CLD_HAS_DLL == 1)
#  if defined (AC_CLD_BUILD_DLL)
#    define AC_CLD_Export ACE_Proper_Export_Flag
#    define AC_CLD_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define AC_CLD_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* AC_CLD_BUILD_DLL */
#    define AC_CLD_Export ACE_Proper_Import_Flag
#    define AC_CLD_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define AC_CLD_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* AC_CLD_BUILD_DLL */
#else /* AC_CLD_HAS_DLL == 1 */
#  define AC_CLD_Export
#  define AC_CLD_SINGLETON_DECLARATION(T)
#  define AC_CLD_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* AC_CLD_HAS_DLL == 1 */

// Set AC_CLD_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (AC_CLD_NTRACE)
#  if (ACE_NTRACE == 1)
#    define AC_CLD_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define AC_CLD_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !AC_CLD_NTRACE */

#if (AC_CLD_NTRACE == 1)
#  define AC_CLD_TRACE(X)
#else /* (AC_CLD_NTRACE == 1) */
#  define AC_CLD_TRACE(X) ACE_TRACE_IMPL(X)
#endif /* (AC_CLD_NTRACE == 1) */

#endif /* AC_CLD_EXPORT_H */

// End of auto generated file.

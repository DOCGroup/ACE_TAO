
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl BASICSP_SVNT
// ------------------------------
#ifndef BASICSP_SVNT_EXPORT_H
#define BASICSP_SVNT_EXPORT_H

#include "ace/config-all.h"

#if !defined (BASICSP_SVNT_HAS_DLL)
#  define BASICSP_SVNT_HAS_DLL 1
#endif /* ! BASICSP_SVNT_HAS_DLL */

#if defined (BASICSP_SVNT_HAS_DLL) && (BASICSP_SVNT_HAS_DLL == 1)
#  if defined (BASICSP_SVNT_BUILD_DLL)
#    define BASICSP_SVNT_Export ACE_Proper_Export_Flag
#    define BASICSP_SVNT_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define BASICSP_SVNT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* BASICSP_SVNT_BUILD_DLL */
#    define BASICSP_SVNT_Export ACE_Proper_Import_Flag
#    define BASICSP_SVNT_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define BASICSP_SVNT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* BASICSP_SVNT_BUILD_DLL */
#else /* BASICSP_SVNT_HAS_DLL == 1 */
#  define BASICSP_SVNT_Export
#  define BASICSP_SVNT_SINGLETON_DECLARATION(T)
#  define BASICSP_SVNT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* BASICSP_SVNT_HAS_DLL == 1 */

// Set BASICSP_SVNT_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (BASICSP_SVNT_NTRACE)
#  if (ACE_NTRACE == 1)
#    define BASICSP_SVNT_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define BASICSP_SVNT_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !BASICSP_SVNT_NTRACE */

#if (BASICSP_SVNT_NTRACE == 1)
#  define BASICSP_SVNT_TRACE(X)
#else /* (BASICSP_SVNT_NTRACE == 1) */
#  define BASICSP_SVNT_TRACE(X) ACE_TRACE_IMPL(X)
#endif /* (BASICSP_SVNT_NTRACE == 1) */

#endif /* BASICSP_SVNT_EXPORT_H */

// End of auto generated file.

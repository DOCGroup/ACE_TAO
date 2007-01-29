
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl generate_export_file.pl ATTRIBUTES_SVNT
// ------------------------------
#ifndef GENERATE_EXPORT_FILE.PL_EXPORT_H
#define GENERATE_EXPORT_FILE.PL_EXPORT_H

#include "ace/config-all.h"

#if !defined (GENERATE_EXPORT_FILE.PL_HAS_DLL)
#  define GENERATE_EXPORT_FILE.PL_HAS_DLL 1
#endif /* ! GENERATE_EXPORT_FILE.PL_HAS_DLL */

#if defined (GENERATE_EXPORT_FILE.PL_HAS_DLL) && (GENERATE_EXPORT_FILE.PL_HAS_DLL == 1)
#  if defined (GENERATE_EXPORT_FILE.PL_BUILD_DLL)
#    define generate_export_file.pl_Export ACE_Proper_Export_Flag
#    define GENERATE_EXPORT_FILE.PL_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define GENERATE_EXPORT_FILE.PL_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* GENERATE_EXPORT_FILE.PL_BUILD_DLL */
#    define generate_export_file.pl_Export ACE_Proper_Import_Flag
#    define GENERATE_EXPORT_FILE.PL_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define GENERATE_EXPORT_FILE.PL_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* GENERATE_EXPORT_FILE.PL_BUILD_DLL */
#else /* GENERATE_EXPORT_FILE.PL_HAS_DLL == 1 */
#  define generate_export_file.pl_Export
#  define GENERATE_EXPORT_FILE.PL_SINGLETON_DECLARATION(T)
#  define GENERATE_EXPORT_FILE.PL_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* GENERATE_EXPORT_FILE.PL_HAS_DLL == 1 */

// Set GENERATE_EXPORT_FILE.PL_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (GENERATE_EXPORT_FILE.PL_NTRACE)
#  if (ACE_NTRACE == 1)
#    define GENERATE_EXPORT_FILE.PL_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define GENERATE_EXPORT_FILE.PL_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !GENERATE_EXPORT_FILE.PL_NTRACE */

#if (GENERATE_EXPORT_FILE.PL_NTRACE == 1)
#  define GENERATE_EXPORT_FILE.PL_TRACE(X)
#else /* (GENERATE_EXPORT_FILE.PL_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define GENERATE_EXPORT_FILE.PL_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (GENERATE_EXPORT_FILE.PL_NTRACE == 1) */

#endif /* GENERATE_EXPORT_FILE.PL_EXPORT_H */

// End of auto generated file.

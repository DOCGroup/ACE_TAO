
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl BBN_UAV_SVNT
// ------------------------------
#ifndef BBN_UAV_SVNT_EXPORT_H
#define BBN_UAV_SVNT_EXPORT_H

#include "ace/config-all.h"

#if !defined (BBN_UAV_SVNT_HAS_DLL)
#  define BBN_UAV_SVNT_HAS_DLL 1
#endif /* ! BBN_UAV_SVNT_HAS_DLL */

#if defined (BBN_UAV_SVNT_HAS_DLL) && (BBN_UAV_SVNT_HAS_DLL == 1)
#  if defined (BBN_UAV_SVNT_BUILD_DLL)
#    define BBN_UAV_SVNT_Export ACE_Proper_Export_Flag
#    define BBN_UAV_SVNT_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define BBN_UAV_SVNT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* BBN_UAV_SVNT_BUILD_DLL */
#    define BBN_UAV_SVNT_Export ACE_Proper_Import_Flag
#    define BBN_UAV_SVNT_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define BBN_UAV_SVNT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* BBN_UAV_SVNT_BUILD_DLL */
#else /* BBN_UAV_SVNT_HAS_DLL == 1 */
#  define BBN_UAV_SVNT_Export
#  define BBN_UAV_SVNT_SINGLETON_DECLARATION(T)
#  define BBN_UAV_SVNT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* BBN_UAV_SVNT_HAS_DLL == 1 */

// Set BBN_UAV_SVNT_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (BBN_UAV_SVNT_NTRACE)
#  if (ACE_NTRACE == 1)
#    define BBN_UAV_SVNT_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define BBN_UAV_SVNT_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !BBN_UAV_SVNT_NTRACE */

#if (BBN_UAV_SVNT_NTRACE == 1)
#  define BBN_UAV_SVNT_TRACE(X)
#else /* (BBN_UAV_SVNT_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define BBN_UAV_SVNT_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (BBN_UAV_SVNT_NTRACE == 1) */

#endif /* BBN_UAV_SVNT_EXPORT_H */

// End of auto generated file.

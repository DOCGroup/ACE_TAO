
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl ATR_SVNT
// ------------------------------
#ifndef ATR_SVNT_EXPORT_H
#define ATR_SVNT_EXPORT_H

#include "ace/config-all.h"

#if !defined (ATR_SVNT_HAS_DLL)
#  define ATR_SVNT_HAS_DLL 1
#endif /* ! ATR_SVNT_HAS_DLL */

#if defined (ATR_SVNT_HAS_DLL) && (ATR_SVNT_HAS_DLL == 1)
#  if defined (ATR_SVNT_BUILD_DLL)
#    define ATR_SVNT_Export ACE_Proper_Export_Flag
#    define ATR_SVNT_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define ATR_SVNT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* ATR_SVNT_BUILD_DLL */
#    define ATR_SVNT_Export ACE_Proper_Import_Flag
#    define ATR_SVNT_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define ATR_SVNT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* ATR_SVNT_BUILD_DLL */
#else /* ATR_SVNT_HAS_DLL == 1 */
#  define ATR_SVNT_Export
#  define ATR_SVNT_SINGLETON_DECLARATION(T)
#  define ATR_SVNT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* ATR_SVNT_HAS_DLL == 1 */

// Set ATR_SVNT_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (ATR_SVNT_NTRACE)
#  if (ACE_NTRACE == 1)
#    define ATR_SVNT_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define ATR_SVNT_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !ATR_SVNT_NTRACE */

#if (ATR_SVNT_NTRACE == 1)
#  define ATR_SVNT_TRACE(X)
#else /* (ATR_SVNT_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define ATR_SVNT_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (ATR_SVNT_NTRACE == 1) */

#endif /* ATR_SVNT_EXPORT_H */

// End of auto generated file.

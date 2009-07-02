
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl bug_2677_regression
// ------------------------------
#ifndef BUG_2677_REGRESSION_EXPORT_H
#define BUG_2677_REGRESSION_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (BUG_2677_REGRESSION_HAS_DLL)
#  define BUG_2677_REGRESSION_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && BUG_2677_REGRESSION_HAS_DLL */

#if !defined (BUG_2677_REGRESSION_HAS_DLL)
#  define BUG_2677_REGRESSION_HAS_DLL 1
#endif /* ! BUG_2677_REGRESSION_HAS_DLL */

#if defined (BUG_2677_REGRESSION_HAS_DLL) && (BUG_2677_REGRESSION_HAS_DLL == 1)
#  if defined (BUG_2677_REGRESSION_BUILD_DLL)
#    define bug_2677_regression_Export ACE_Proper_Export_Flag
#    define BUG_2677_REGRESSION_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define BUG_2677_REGRESSION_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* BUG_2677_REGRESSION_BUILD_DLL */
#    define bug_2677_regression_Export ACE_Proper_Import_Flag
#    define BUG_2677_REGRESSION_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define BUG_2677_REGRESSION_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* BUG_2677_REGRESSION_BUILD_DLL */
#else /* BUG_2677_REGRESSION_HAS_DLL == 1 */
#  define bug_2677_regression_Export
#  define BUG_2677_REGRESSION_SINGLETON_DECLARATION(T)
#  define BUG_2677_REGRESSION_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* BUG_2677_REGRESSION_HAS_DLL == 1 */

// Set BUG_2677_REGRESSION_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (BUG_2677_REGRESSION_NTRACE)
#  if (ACE_NTRACE == 1)
#    define BUG_2677_REGRESSION_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define BUG_2677_REGRESSION_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !BUG_2677_REGRESSION_NTRACE */

#if (BUG_2677_REGRESSION_NTRACE == 1)
#  define BUG_2677_REGRESSION_TRACE(X)
#else /* (BUG_2677_REGRESSION_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define BUG_2677_REGRESSION_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (BUG_2677_REGRESSION_NTRACE == 1) */

#endif /* BUG_2677_REGRESSION_EXPORT_H */

// End of auto generated file.

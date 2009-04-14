
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl -d Current_Test
// ------------------------------
#ifndef CURRENT_TEST_EXPORT_H
#define CURRENT_TEST_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (CURRENT_TEST_HAS_DLL)
#  define CURRENT_TEST_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && CURRENT_TEST_HAS_DLL */

#if !defined (CURRENT_TEST_HAS_DLL)
#  define CURRENT_TEST_HAS_DLL 1
#endif /* ! CURRENT_TEST_HAS_DLL */

#if defined (CURRENT_TEST_HAS_DLL) && (CURRENT_TEST_HAS_DLL == 1)
#  if defined (CURRENT_TEST_BUILD_DLL)
#    define Current_Test_Export ACE_Proper_Export_Flag
#    define CURRENT_TEST_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define CURRENT_TEST_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* CURRENT_TEST_BUILD_DLL */
#    define Current_Test_Export ACE_Proper_Import_Flag
#    define CURRENT_TEST_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define CURRENT_TEST_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* CURRENT_TEST_BUILD_DLL */
#else /* CURRENT_TEST_HAS_DLL == 1 */
#  define Current_Test_Export
#  define CURRENT_TEST_SINGLETON_DECLARATION(T)
#  define CURRENT_TEST_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* CURRENT_TEST_HAS_DLL == 1 */

// Set CURRENT_TEST_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (CURRENT_TEST_NTRACE)
#  if (ACE_NTRACE == 1)
#    define CURRENT_TEST_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define CURRENT_TEST_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !CURRENT_TEST_NTRACE */

#if (CURRENT_TEST_NTRACE == 1)
#  define CURRENT_TEST_TRACE(X)
#else /* (CURRENT_TEST_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define CURRENT_TEST_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (CURRENT_TEST_NTRACE == 1) */

#endif /* CURRENT_TEST_EXPORT_H */

// End of auto generated file.


// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl Test_Output
// ------------------------------
#ifndef TEST_OUTPUT_EXPORT_H
#define TEST_OUTPUT_EXPORT_H

#include "ace/config-all.h"

#if !defined (TEST_OUTPUT_HAS_DLL)
#  define TEST_OUTPUT_HAS_DLL 1
#endif /* ! TEST_OUTPUT_HAS_DLL */

#if defined (TEST_OUTPUT_HAS_DLL) && (TEST_OUTPUT_HAS_DLL == 1)
#  if defined (TEST_OUTPUT_BUILD_DLL)
#    define Test_Output_Export ACE_Proper_Export_Flag
#    define TEST_OUTPUT_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define TEST_OUTPUT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* TEST_OUTPUT_BUILD_DLL */
#    define Test_Output_Export ACE_Proper_Import_Flag
#    define TEST_OUTPUT_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define TEST_OUTPUT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* TEST_OUTPUT_BUILD_DLL */
#else /* TEST_OUTPUT_HAS_DLL == 1 */
#  define Test_Output_Export
#  define TEST_OUTPUT_SINGLETON_DECLARATION(T)
#  define TEST_OUTPUT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* TEST_OUTPUT_HAS_DLL == 1 */

// Set TEST_OUTPUT_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (TEST_OUTPUT_NTRACE)
#  if (ACE_NTRACE == 1)
#    define TEST_OUTPUT_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define TEST_OUTPUT_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !TEST_OUTPUT_NTRACE */

#if (TEST_OUTPUT_NTRACE == 1)
#  define TEST_OUTPUT_TRACE(X)
#else /* (TEST_OUTPUT_NTRACE == 1) */
#  define TEST_OUTPUT_TRACE(X) ACE_TRACE_IMPL(X)
#endif /* (TEST_OUTPUT_NTRACE == 1) */

#endif /* TEST_OUTPUT_EXPORT_H */

// End of auto generated file.


// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl -s DLL_Test_Parent
// ------------------------------
#ifndef DLL_TEST_PARENT_EXPORT_H
#define DLL_TEST_PARENT_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (DLL_TEST_PARENT_HAS_DLL)
#  define DLL_TEST_PARENT_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && DLL_TEST_PARENT_HAS_DLL */

#if !defined (DLL_TEST_PARENT_HAS_DLL)
#  define DLL_TEST_PARENT_HAS_DLL 1
#endif /* ! DLL_TEST_PARENT_HAS_DLL */

#if defined (DLL_TEST_PARENT_HAS_DLL) && (DLL_TEST_PARENT_HAS_DLL == 1)
#  if defined (DLL_TEST_PARENT_BUILD_DLL)
#    define DLL_Test_Parent_Export ACE_Proper_Export_Flag
#    define DLL_TEST_PARENT_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define DLL_TEST_PARENT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* DLL_TEST_PARENT_BUILD_DLL */
#    define DLL_Test_Parent_Export ACE_Proper_Import_Flag
#    define DLL_TEST_PARENT_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define DLL_TEST_PARENT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* DLL_TEST_PARENT_BUILD_DLL */
#else /* DLL_TEST_PARENT_HAS_DLL == 1 */
#  define DLL_Test_Parent_Export
#  define DLL_TEST_PARENT_SINGLETON_DECLARATION(T)
#  define DLL_TEST_PARENT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* DLL_TEST_PARENT_HAS_DLL == 1 */

// Set DLL_TEST_PARENT_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (DLL_TEST_PARENT_NTRACE)
#  if (ACE_NTRACE == 1)
#    define DLL_TEST_PARENT_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define DLL_TEST_PARENT_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !DLL_TEST_PARENT_NTRACE */

#if (DLL_TEST_PARENT_NTRACE == 1)
#  define DLL_TEST_PARENT_TRACE(X)
#else /* (DLL_TEST_PARENT_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define DLL_TEST_PARENT_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (DLL_TEST_PARENT_NTRACE == 1) */

#endif /* DLL_TEST_PARENT_EXPORT_H */

// End of auto generated file.

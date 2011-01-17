
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl
// ------------------------------
#ifndef EC_TEST_EXPORT_H
#define EC_TEST_EXPORT_H

#include "ace/config-all.h"

#if defined (TAO_AS_STATIC_LIBS)
#  if !defined (EC_TEST_HAS_DLL)
#    define EC_TEST_HAS_DLL 0
#  endif /* ! EC_TEST_HAS_DLL */
#else
#  if !defined (EC_TEST_HAS_DLL)
#    define EC_TEST_HAS_DLL 1
#  endif /* ! EC_TEST_HAS_DLL */
#endif

#if defined (EC_TEST_HAS_DLL) && (EC_TEST_HAS_DLL == 1)
#  if defined (EC_TEST_BUILD_DLL)
#    define EC_Test_Export ACE_Proper_Export_Flag
#    define EC_TEST_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define EC_TEST_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* EC_TEST_BUILD_DLL */
#    define EC_Test_Export ACE_Proper_Import_Flag
#    define EC_TEST_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define EC_TEST_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* EC_TEST_BUILD_DLL */
#else /* EC_TEST_HAS_DLL == 1 */
#  define EC_Test_Export
#  define EC_TEST_SINGLETON_DECLARATION(T)
#  define EC_TEST_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* EC_TEST_HAS_DLL == 1 */

#endif /* EC_TEST_EXPORT_H */

// End of auto generated file.

// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by
// generate_export_file.pl
// ------------------------------
#if !defined (TEST_EXPORT_H)
#define TEST_EXPORT_H

#include "ace/config-all.h"

#if !defined (TEST_HAS_DLL)
#define TEST_HAS_DLL 1
#endif /* ! TEST_HAS_DLL */

#if defined (TEST_HAS_DLL)
#  if (TEST_HAS_DLL == 1)
#    if defined (TEST_BUILD_DLL)
#      define Test_Export ACE_Proper_Export_Flag
#      define TEST_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#      define TEST_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#    else
#      define Test_Export ACE_Proper_Import_Flag
#      define TEST_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#      define TEST_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#    endif /* TEST_BUILD_DLL */
#  else
#    define Test_Export
#    define TEST_SINGLETON_DECLARATION(T)
#    define TEST_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif   /* ! TEST_HAS_DLL == 1 */
#else
#  define Test_Export
#  define TEST_SINGLETON_DECLARATION(T)
#  define TEST_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif     /* TEST_HAS_DLL */

#endif     /* TEST_EXPORT_H */

// End of auto generated file.

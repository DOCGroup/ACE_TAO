
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl
// ------------------------------
#ifndef TEST_CLIENT_MODULE_EXPORT_H
#define TEST_CLIENT_MODULE_EXPORT_H

#include "ace/config-all.h"

#if defined (TAO_AS_STATIC_LIBS)
#  if !defined (TEST_CLIENT_MODULE_HAS_DLL)
#    define TEST_CLIENT_MODULE_HAS_DLL 0
#  endif /* ! TEST_CLIENT_MODULE_HAS_DLL */
#else
#  if !defined (TEST_CLIENT_MODULE_HAS_DLL)
#    define TEST_CLIENT_MODULE_HAS_DLL 1
#  endif /* ! TEST_CLIENT_MODULE_HAS_DLL */
#endif

#if defined (TEST_CLIENT_MODULE_HAS_DLL) && (TEST_CLIENT_MODULE_HAS_DLL == 1)
#  if defined (TEST_CLIENT_MODULE_BUILD_DLL)
#    define Test_Client_Module_Export ACE_Proper_Export_Flag
#    define TEST_CLIENT_MODULE_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define TEST_CLIENT_MODULE_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* TEST_CLIENT_MODULE_BUILD_DLL */
#    define Test_Client_Module_Export ACE_Proper_Import_Flag
#    define TEST_CLIENT_MODULE_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define TEST_CLIENT_MODULE_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* TEST_CLIENT_MODULE_BUILD_DLL */
#else /* TEST_CLIENT_MODULE_HAS_DLL == 1 */
#  define Test_Client_Module_Export
#  define TEST_CLIENT_MODULE_SINGLETON_DECLARATION(T)
#  define TEST_CLIENT_MODULE_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* TEST_CLIENT_MODULE_HAS_DLL == 1 */

#endif /* TEST_CLIENT_MODULE_EXPORT_H */

// End of auto generated file.

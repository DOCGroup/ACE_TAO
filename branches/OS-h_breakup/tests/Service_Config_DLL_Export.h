
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl
// ------------------------------
#ifndef SERVICE_CONFIG_DLL_EXPORT_H
#define SERVICE_CONFIG_DLL_EXPORT_H

#include "ace/config-all.h"

#if !defined (SERVICE_CONFIG_DLL_HAS_DLL)
#  define SERVICE_CONFIG_DLL_HAS_DLL 1
#endif /* ! TEST_HAS_DLL */

#if defined (SERVICE_CONFIG_DLL_HAS_DLL) && (SERVICE_CONFIG_DLL_HAS_DLL == 1)
#  if defined (SERVICE_CONFIG_DLL_BUILD_DLL)
#    define Service_Config_DLL_Export ACE_Proper_Export_Flag
#    define TEST_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define TEST_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* SERVICE_CONFIG_DLL_BUILD_DLL */
#    define Service_Config_DLL_Export ACE_Proper_Import_Flag
#    define TEST_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define TEST_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* SERVICE_CONFIG_DLL_BUILD_DLL */
#else /* SERVICE_CONFIG_DLL_HAS_DLL == 1 */
#  define Service_Config_DLL_Export
#  define TEST_SINGLETON_DECLARATION(T)
#  define TEST_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* SERVICE_CONFIG_DLL_HAS_DLL == 1 */

#endif /* SERVICE_CONFIG_DLL_EXPORT_H */

// End of auto generated file.

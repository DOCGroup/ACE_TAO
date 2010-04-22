// -*- C++ -*-

//=============================================================================
/**
 *  @file    MC_Test_Utilities_export.h
 *
 *  $Id$
 *
 *  @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================

// Definition for export directives.
// ------------------------------
#ifndef MC_TEST_UTILITIES_EXPORT_H
#define MC_TEST_UTILITIES_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS)
# if !defined (MC_TEST_UTILITIES_HAS_DLL)
#   define MC_TEST_UTILITIES_HAS_DLL 0
# endif /* ! MC_TEST_UTILITIES_HAS_DLL */
#else
# if !defined (MC_TEST_UTILITIES_HAS_DLL)
#   define MC_TEST_UTILITIES_HAS_DLL 1
# endif /* ! MC_TEST_UTILITIES_HAS_DLL */
#endif /* ACE_AS_STATIC_LIB */

#if defined (MC_TEST_UTILITIES_HAS_DLL) && (MC_TEST_UTILITIES_HAS_DLL == 1)
#  if defined (MC_TEST_UTILITIES_BUILD_DLL)
#    define MC_TEST_UTILITIES_Export ACE_Proper_Export_Flag
#    define MC_TEST_UTILITIES_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define MC_TEST_UTILITIES_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* MC_TEST_UTILITIES_BUILD_DLL */
#    define MC_TEST_UTILITIES_Export ACE_Proper_Import_Flag
#    define MC_TEST_UTILITIES_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define MC_TEST_UTILITIES_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* MC_TEST_UTILITIES_BUILD_DLL */
#else /* MC_TEST_UTILITIES_HAS_DLL == 1 */
#  define MC_TEST_UTILITIES_Export
#  define MC_TEST_UTILITIES_SINGLETON_DECLARATION(T)
#  define MC_TEST_UTILITIES_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* MC_TEST_UTILITIES_HAS_DLL == 1 */

#endif /* MC_TEST_UTILITIES_EXPORT_H */


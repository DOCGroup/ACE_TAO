
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl
// ------------------------------
#ifndef CEC_TEST_EXPORT_H
#define CEC_TEST_EXPORT_H

#include "ace/config-all.h"

#if defined (TAO_AS_STATIC_LIBS)
#  if !defined (CEC_TEST_HAS_DLL)
#    define CEC_TEST_HAS_DLL 0
#  endif /* ! CEC_TEST_HAS_DLL */
#else
#  if !defined (CEC_TEST_HAS_DLL)
#    define CEC_TEST_HAS_DLL 1
#  endif /* ! CEC_TEST_HAS_DLL */
#endif

#if defined (CEC_TEST_HAS_DLL) && (CEC_TEST_HAS_DLL == 1)
#  if defined (CEC_TEST_BUILD_DLL)
#    define CEC_Test_Export ACE_Proper_Export_Flag
#    define CEC_TEST_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define CEC_TEST_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* CEC_TEST_BUILD_DLL */
#    define CEC_Test_Export ACE_Proper_Import_Flag
#    define CEC_TEST_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define CEC_TEST_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* CEC_TEST_BUILD_DLL */
#else /* CEC_TEST_HAS_DLL == 1 */
#  define CEC_Test_Export
#  define CEC_TEST_SINGLETON_DECLARATION(T)
#  define CEC_TEST_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* CEC_TEST_HAS_DLL == 1 */

#endif /* CEC_TEST_EXPORT_H */

// End of auto generated file.


// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl
// ------------------------------
#ifndef QUOTER_COMMON_EXPORT_H
#define QUOTER_COMMON_EXPORT_H

#include "ace/config-all.h"

#if defined (TAO_AS_STATIC_LIBS)
#  if !defined (QUOTER_COMMON_HAS_DLL)
#    define QUOTER_COMMON_HAS_DLL 0
#  endif /* ! QUOTER_COMMON_HAS_DLL */
#else
#  if !defined (QUOTER_COMMON_HAS_DLL)
#    define QUOTER_COMMON_HAS_DLL 1
#  endif /* ! QUOTER_COMMON_HAS_DLL */
#endif

#if defined (QUOTER_COMMON_HAS_DLL) && (QUOTER_COMMON_HAS_DLL == 1)
#  if defined (QUOTER_COMMON_BUILD_DLL)
#    define QUOTER_COMMON_Export ACE_Proper_Export_Flag
#    define QUOTER_COMMON_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define QUOTER_COMMON_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* QUOTER_COMMON_BUILD_DLL */
#    define QUOTER_COMMON_Export ACE_Proper_Import_Flag
#    define QUOTER_COMMON_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define QUOTER_COMMON_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* QUOTER_COMMON_BUILD_DLL */
#else /* QUOTER_COMMON_HAS_DLL == 1 */
#  define QUOTER_COMMON_Export
#  define QUOTER_COMMON_SINGLETON_DECLARATION(T)
#  define QUOTER_COMMON_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* QUOTER_COMMON_HAS_DLL == 1 */

#endif /* QUOTER_COMMON_EXPORT_H */

// End of auto generated file.

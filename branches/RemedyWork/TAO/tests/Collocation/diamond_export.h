
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl
// ------------------------------
#ifndef DIAMOND_EXPORT_H
#define DIAMOND_EXPORT_H

#include "ace/config-all.h"

#if defined (TAO_AS_STATIC_LIBS)
#  if !defined (DIAMOND_HAS_DLL)
#    define DIAMOND_HAS_DLL 0
#  endif /* ! DIAMOND_HAS_DLL */
#else
#  if !defined (DIAMOND_HAS_DLL)
#    define DIAMOND_HAS_DLL 1
#  endif /* ! DIAMOND_HAS_DLL */
#endif

#if defined (DIAMOND_HAS_DLL) && (DIAMOND_HAS_DLL == 1)
#  if defined (DIAMOND_BUILD_DLL)
#    define Diamond_Export ACE_Proper_Export_Flag
#    define DIAMOND_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define DIAMOND_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* DIAMOND_BUILD_DLL */
#    define Diamond_Export ACE_Proper_Import_Flag
#    define DIAMOND_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define DIAMOND_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* DIAMOND_BUILD_DLL */
#else /* DIAMOND_HAS_DLL == 1 */
#  define Diamond_Export
#  define DIAMOND_SINGLETON_DECLARATION(T)
#  define DIAMOND_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* DIAMOND_HAS_DLL == 1 */

#endif /* DIAMOND_EXPORT_H */

// End of auto generated file.

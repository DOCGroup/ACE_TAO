// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by
// generate_export_file.pl
// ------------------------------
#if !defined (TAO_IOR_DLL_EXPORT_H)
#define TAO_IOR_DLL_EXPORT_H

#include "ace/config-all.h"

#if !defined (TAO_IOR_DLL_HAS_DLL)
#define TAO_IOR_DLL_HAS_DLL 1
#endif /* ! TAO_IOR_DLL_HAS_DLL */

#if defined (TAO_IOR_DLL_HAS_DLL)
#  if (TAO_IOR_DLL_HAS_DLL == 1)
#    if defined (TAO_IOR_DLL_BUILD_DLL)
#      define TAO_IOR_DLL_Export ACE_Proper_Export_Flag
#      define TAO_IOR_DLL_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#      define TAO_IOR_DLL_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#    else
#      define TAO_IOR_DLL_Export ACE_Proper_Import_Flag
#      define TAO_IOR_DLL_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#      define TAO_IOR_DLL_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#    endif /* TAO_IOR_DLL_BUILD_DLL */
#  else
#    define TAO_IOR_DLL_Export
#    define TAO_IOR_DLL_SINGLETON_DECLARATION(T)
#    define TAO_IOR_DLL_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif   /* ! TAO_IOR_DLL_HAS_DLL == 1 */
#else
#  define TAO_IOR_DLL_Export
#  define TAO_IOR_DLL_SINGLETON_DECLARATION(T)
#  define TAO_IOR_DLL_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif     /* TAO_IOR_DLL_HAS_DLL */

#endif     /* TAO_IOR_DLL_EXPORT_H */

// End of auto generated file.

// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by
// ${ACE_ROOT}/GenExportH.BAT
// ------------------------------
#if !defined (Diamond_EXPORT_H)
#define Diamond_EXPORT_H

#include "ace/OS.h"

#if !defined (Diamond_HAS_DLL)
#define Diamond_HAS_DLL 1
#endif /* ! Diamond_HAS_DLL */

#if defined (Diamond_HAS_DLL)
#  if (Diamond_HAS_DLL == 1)
#    if defined (Diamond_BUILD_DLL)
#      define Diamond_Export ACE_Proper_Export_Flag
#      define Diamond_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    else
#      define Diamond_Export ACE_Proper_Import_Flag
#      define Diamond_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    endif /* Diamond_BUILD_DLL */
#  else
#    define Diamond_Export
#    define Diamond_SINGLETON_DECLARATION(T)
#  endif   /* ! Diamond_HAS_DLL == 1 */
#else
#  define Diamond_Export
#  define Diamond_SINGLETON_DECLARATION(T)
#endif     /* Diamond_HAS_DLL */

#endif     /* Diamond_EXPORT_H */

// End of auto generated file.

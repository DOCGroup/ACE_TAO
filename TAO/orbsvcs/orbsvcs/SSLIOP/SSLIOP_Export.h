// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by
// ${ACE_ROOT}/GenExportH.BAT
// ------------------------------
#ifndef TAO_SSLIOP_EXPORT_H
#define TAO_SSLIOP_EXPORT_H

#include "ace/OS.h"

#if !defined (TAO_SSLIOP_HAS_DLL)
#define TAO_SSLIOP_HAS_DLL 1
#endif /* ! TAO_SSLIOP_HAS_DLL */

#if defined (TAO_SSLIOP_HAS_DLL)
#  if (TAO_SSLIOP_HAS_DLL == 1)
#    if defined (TAO_SSLIOP_BUILD_DLL)
#      define TAO_SSLIOP_Export ACE_Proper_Export_Flag
#      define TAO_SSLIOP_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    else
#      define TAO_SSLIOP_Export ACE_Proper_Import_Flag
#      define TAO_SSLIOP_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    endif /* TAO_SSLIOP_BUILD_DLL */
#  else
#    define TAO_SSLIOP_Export
#    define TAO_SSLIOP_SINGLETON_DECLARATION(T)
#  endif   /* ! TAO_SSLIOP_HAS_DLL == 1 */
#else
#  define TAO_SSLIOP_Export
#  define TAO_SSLIOP_SINGLETON_DECLARATION(T)
#endif     /* TAO_SSLIOP_HAS_DLL */

#endif     /* TAO_SSLIOP_EXPORT_H */

// End of auto generated file.

// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by
// ${ACE_ROOT}/GenExportH.BAT
// ------------------------------
#ifndef TAO_AV_EXPORT_H
#define TAO_AV_EXPORT_H

#include "ace/OS.h"

#if !defined (TAO_AV_HAS_DLL)
#define TAO_AV_HAS_DLL 1
#endif /* ! TAO_AV_HAS_DLL */

#if defined (TAO_AV_HAS_DLL)
#  if (TAO_AV_HAS_DLL == 1)
#    if defined (TAO_AV_BUILD_DLL)
#      define TAO_AV_Export ACE_Proper_Export_Flag
#      define TAO_AV_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#      define TAO_AV_SINGLETON_DECLARE ACE_EXPORT_SINGLETON_DECLARE
#    else
#      define TAO_AV_Export ACE_Proper_Import_Flag
#      define TAO_AV_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#      define TAO_AV_SINGLETON_DECLARE ACE_IMPORT_SINGLETON_DECLARE
#    endif /* TAO_AV_BUILD_DLL */
#  else
#    define TAO_AV_Export
#    define TAO_AV_SINGLETON_DECLARATION(T)
#    define TAO_AV_SINGLETON_DECLARE
#  endif   /* ! TAO_AV_HAS_DLL == 1 */
#else
#  define TAO_AV_Export
#  define TAO_AV_SINGLETON_DECLARATION(T)
#  define TAO_AV_SINGLETON_DECLARE
#endif     /* TAO_AV_HAS_DLL */

#endif     /* TAO_AV_EXPORT_H */

// End of auto generated file.

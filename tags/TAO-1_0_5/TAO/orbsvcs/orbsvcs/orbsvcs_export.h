// $Id$

// Definition for Win32 Export directives.
// This file is generated automatically by
// ${TAO_ROOT}/TAO_IDL/GenExportH.BAT
// ------------------------------
#ifndef TAO_ORBSVCS_EXPORT_H
#define TAO_ORBSVCS_EXPORT_H

#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (TAO_ORBSVCS_HAS_DLL)
#  if (TAO_ORBSVCS_HAS_DLL == 1)
#    if defined (TAO_ORBSVCS_BUILD_DLL)
#      define TAO_ORBSVCS_Export ACE_Proper_Export_Flag
#      define TAO_ORBSVCS_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    else
#      define TAO_ORBSVCS_Export ACE_Proper_Import_Flag
#      define TAO_ORBSVCS_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    endif /* TAO_ORBSVCS_BUILD_DLL */
#  else
#    define TAO_ORBSVCS_Export
#    define TAO_ORBSVCS_SINGLETON_DECLARATION(T)
#  endif   /* ! TAO_ORBSVCS_HAS_DLL == 1 */
#else
#  define TAO_ORBSVCS_Export
#  define TAO_ORBSVCS_SINGLETON_DECLARATION(T)
#endif     /* TAO_ORBSVCS_HAS_DLL */

#endif     /* TAO_ORBSVCS_EXPORT_H */
           // End of auto generated file.

// $Id$

// Definition for Win32 Export directives.
// This file is generated automatically by
// ${TAO_ROOT}/TAO_IDL/GenExportH.BAT
// ------------------------------
#ifndef GENERIC_SERVANT_EXPORT_H
#define GENERIC_SERVANT_EXPORT_H

#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (GENERIC_SERVANT_HAS_DLL)
#  if (GENERIC_SERVANT_HAS_DLL == 1)
#    if defined (GENERIC_SERVANT_BUILD_DLL)
#      define GENERIC_SERVANT_Export ACE_Proper_Export_Flag
#      define GENERIC_SERVANT_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    else
#      define GENERIC_SERVANT_Export ACE_Proper_Import_Flag
#      define GENERIC_SERVANT_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    endif /* GENERIC_SERVANT_BUILD_DLL */
#  else
#    define GENERIC_SERVANT_Export
#    define GENERIC_SERVANT_SINGLETON_DECLARATION(T)
#  endif   /* ! GENERIC_SERVANT_HAS_DLL == 1 */
#else
#  define GENERIC_SERVANT_Export
#  define GENERIC_SERVANT_SINGLETON_DECLARATION(T)
#endif     /* GENERIC_SERVANT_HAS_DLL */

#endif     /* GENERIC_SERVANT_EXPORT_H */
           // End of auto generated file.

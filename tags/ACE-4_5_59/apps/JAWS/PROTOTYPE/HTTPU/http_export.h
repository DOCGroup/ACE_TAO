// $Id$

// Definition for Win32 Export directives.
// This file is generated automatically by
// ${TAO_ROOT}/TAO_IDL/GenExportH.BAT
// ------------------------------
#if !defined (HTTPU_EXPORT_H)
#define HTTPU_EXPORT_H

#include "ace/OS.h"

#if defined (HTTPU_HAS_DLL)
#  if (HTTPU_HAS_DLL == 1)
#    if defined (HTTPU_BUILD_DLL)
#      define HTTPU_Export ACE_Proper_Export_Flag
#      define HTTPU_SINGLETON_DECLARATION(T) \
              ACE_EXPORT_SINGLETON_DECLARATION (T)
#    else
#      define HTTPU_Export ACE_Proper_Import_Flag
#      define HTTPU_SINGLETON_DECLARATION(T) \
              ACE_IMPORT_SINGLETON_DECLARATION (T)
#    endif /* HTTPU_BUILD_DLL */
#  else
#    define HTTPU_Export
#    define HTTPU_SINGLETON_DECLARATION(T)
#  endif   /* ! HTTPU_HAS_DLL == 1 */
#else
#  define HTTPU_Export
#  define HTTPU_SINGLETON_DECLARATION(T)
#endif     /* HTTPU_HAS_DLL */

#endif     /* HTTPU_EXPORT_H */
           // End of auto generated file.


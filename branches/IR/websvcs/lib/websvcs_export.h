// $Id$

// Definition for Win32 Export directives.
// This file is generated automatically by
// ${TAO_ROOT}/TAO_IDL/GenExportH.BAT
// ------------------------------
#ifndef ACE_WEBSVCS_EXPORT_H
#define ACE_WEBSVCS_EXPORT_H

#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (ACE_WEBSVCS_HAS_DLL)
#  if (ACE_WEBSVCS_HAS_DLL == 1)
#    if defined (ACE_WEBSVCS_BUILD_DLL)
#      define ACE_WEBSVCS_Export ACE_Proper_Export_Flag
#      define ACE_WEBSVCS_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    else
#      define ACE_WEBSVCS_Export ACE_Proper_Import_Flag
#      define ACE_WEBSVCS_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    endif /* ACE_WEBSVCS_BUILD_DLL */
#  else
#    define ACE_WEBSVCS_Export
#    define ACE_WEBSVCS_SINGLETON_DECLARATION(T)
#  endif   /* ! ACE_WEBSVCS_HAS_DLL == 1 */
#else
#  define ACE_WEBSVCS_Export
#  define ACE_WEBSVCS_SINGLETON_DECLARATION(T)
#endif     /* ACE_WEBSVCS_HAS_DLL */

#endif     /* ACE_WEBSVCS_EXPORT_H */
           // End of auto generated file.

// $Id$

// Definition for Win32 Export directives.
// This file is generated automatically by
// ${ACE_ROOT}/GenExportH.BAT
// ------------------------------
#if !defined (Alternate_Resource_Factory_EXPORT_H)
#define Alternate_Resource_Factory_EXPORT_H

#include "ace/OS.h"

#if defined (Alternate_Resource_Factory_HAS_DLL)
#  if (Alternate_Resource_Factory_HAS_DLL == 1)
#    if defined (Alternate_Resource_Factory_BUILD_DLL)
#      define Alternate_Resource_Factory_Export ACE_Proper_Export_Flag
#      define Alternate_Resource_Factory_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    else
#      define Alternate_Resource_Factory_Export ACE_Proper_Import_Flag
#      define Alternate_Resource_Factory_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    endif /* Alternate_Resource_Factory_BUILD_DLL */
#  else
#    define Alternate_Resource_Factory_Export
#    define Alternate_Resource_Factory_SINGLETON_DECLARATION(T)
#  endif   /* ! Alternate_Resource_Factory_HAS_DLL == 1 */
#else
#  define Alternate_Resource_Factory_Export
#  define Alternate_Resource_Factory_SINGLETON_DECLARATION(T)
#endif     /* Alternate_Resource_Factory_HAS_DLL */

#endif     /* Alternate_Resource_Factory_EXPORT_H */

// End of auto generated file.

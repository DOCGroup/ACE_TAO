// $Id$

// Definition for Win32 Export directives.
// This file is generated automatically by
// ${ACE_ROOT}/GenExportH.BAT
// ------------------------------
#ifndef Alt_Resource_Factory_EXPORT_H
#define Alt_Resource_Factory_EXPORT_H

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (TAO_AS_STATIC_LIBS)
#  if !defined (Alt_Resource_Factory_HAS_DLL)
#    define Alt_Resource_Factory_HAS_DLL 0
#  endif /* ! Alt_Resource_Factory_HAS_DLL */
#else
#  if !defined (Alt_Resource_Factory_HAS_DLL)
#    define Alt_Resource_Factory_HAS_DLL 1
#  endif /* ! Alt_Resource_Factory_HAS_DLL */
#endif

#if defined (Alt_Resource_Factory_HAS_DLL)
#  if (Alt_Resource_Factory_HAS_DLL == 1)
#    if defined (Alt_Resource_Factory_BUILD_DLL)
#      define Alt_Resource_Factory_Export ACE_Proper_Export_Flag
#      define Alt_Resource_Factory_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    else
#      define Alt_Resource_Factory_Export ACE_Proper_Import_Flag
#      define Alt_Resource_Factory_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    endif /* Alt_Resource_Factory_BUILD_DLL */
#  else
#    define Alt_Resource_Factory_Export
#    define Alt_Resource_Factory_SINGLETON_DECLARATION(T)
#  endif   /* ! Alt_Resource_Factory_HAS_DLL == 1 */
#else
#  define Alt_Resource_Factory_Export
#  define Alt_Resource_Factory_SINGLETON_DECLARATION(T)
#endif     /* Alt_Resource_Factory_HAS_DLL */

#endif     /* Alt_Resource_Factory_EXPORT_H */

// End of auto generated file.

// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by
// ${ACE_ROOT}/GenExportH.BAT
// ------------------------------
#if !defined (EC_Test_EXPORT_H)
#define EC_Test_EXPORT_H

#include "ace/OS.h"

#if !defined (EC_Test_HAS_DLL)
#define EC_Test_HAS_DLL 1
#endif /* ! EC_Test_HAS_DLL */

#if defined (EC_Test_HAS_DLL)
#  if (EC_Test_HAS_DLL == 1)
#    if defined (EC_Test_BUILD_DLL)
#      define EC_Test_Export ACE_Proper_Export_Flag
#      define EC_Test_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    else
#      define EC_Test_Export ACE_Proper_Import_Flag
#      define EC_Test_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    endif /* EC_Test_BUILD_DLL */
#  else
#    define EC_Test_Export
#    define EC_Test_SINGLETON_DECLARATION(T)
#  endif   /* ! EC_Test_HAS_DLL == 1 */
#else
#  define EC_Test_Export
#  define EC_Test_SINGLETON_DECLARATION(T)
#endif     /* EC_Test_HAS_DLL */

#endif     /* EC_Test_EXPORT_H */

// End of auto generated file.

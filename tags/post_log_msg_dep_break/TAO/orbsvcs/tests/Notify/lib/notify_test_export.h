// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by
// ${ACE_ROOT}/GenExportH.BAT
// ------------------------------
#if !defined (Notify_Test_EXPORT_H)
#define Notify_Test_EXPORT_H

#include "ace/OS.h"

#if !defined (Notify_Test_HAS_DLL)
#define Notify_Test_HAS_DLL 1
#endif /* ! Notify_Test_HAS_DLL */

#if defined (Notify_Test_HAS_DLL)
#  if (Notify_Test_HAS_DLL == 1)
#    if defined (Notify_Test_BUILD_DLL)
#      define Notify_Test_Export ACE_Proper_Export_Flag
#      define Notify_Test_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    else
#      define Notify_Test_Export ACE_Proper_Import_Flag
#      define Notify_Test_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    endif /* Notify_Test_BUILD_DLL */
#  else
#    define Notify_Test_Export
#    define Notify_Test_SINGLETON_DECLARATION(T)
#  endif   /* ! Notify_Test_HAS_DLL == 1 */
#else
#  define Notify_Test_Export
#  define Notify_Test_SINGLETON_DECLARATION(T)
#endif     /* Notify_Test_HAS_DLL */

#endif     /* Notify_Test_EXPORT_H */

// End of auto generated file.

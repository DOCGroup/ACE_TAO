// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by
// ${ACE_ROOT}/GenExportH.BAT
// ------------------------------
#if !defined (MY_Stub_EXPORT_H)
#define MY_Stub_EXPORT_H

#include "ace/OS.h"

#if !defined (MY_Stub_HAS_DLL)
#define MY_Stub_HAS_DLL 1
#endif /* ! MY_Stub_HAS_DLL */

#if defined (MY_Stub_HAS_DLL)
#  if (MY_Stub_HAS_DLL == 1)
#    if defined (MY_Stub_BUILD_DLL)
#      define MY_Stub_Export ACE_Proper_Export_Flag
#      define MY_Stub_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    else
#      define MY_Stub_Export ACE_Proper_Import_Flag
#      define MY_Stub_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    endif /* MY_Stub_BUILD_DLL */
#  else
#    define MY_Stub_Export
#    define MY_Stub_SINGLETON_DECLARATION(T)
#  endif   /* ! MY_Stub_HAS_DLL == 1 */
#else
#  define MY_Stub_Export
#  define MY_Stub_SINGLETON_DECLARATION(T)
#endif     /* MY_Stub_HAS_DLL */

#endif     /* MY_Stub_EXPORT_H */

// End of auto generated file.

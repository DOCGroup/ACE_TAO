// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by
// ${ACE_ROOT}/GenExportH.BAT
// ------------------------------
#if !defined (OLT_EXPORT_H)
#define OLT_EXPORT_H

#include "ace/OS.h"

#if !defined (OLT_HAS_DLL)
#define OLT_HAS_DLL 1
#endif /* ! OLT_HAS_DLL */

#if defined (OLT_HAS_DLL)
#  if (OLT_HAS_DLL == 1)
#    if defined (OLT_BUILD_DLL)
#      define OLT_Export ACE_Proper_Export_Flag
#      define OLT_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    else
#      define OLT_Export ACE_Proper_Import_Flag
#      define OLT_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    endif /* OLT_BUILD_DLL */
#  else
#    define OLT_Export
#    define OLT_SINGLETON_DECLARATION(T)
#  endif   /* ! OLT_HAS_DLL == 1 */
#else
#  define OLT_Export
#  define OLT_SINGLETON_DECLARATION(T)
#endif     /* OLT_HAS_DLL */

#endif     /* OLT_EXPORT_H */

// End of auto generated file.

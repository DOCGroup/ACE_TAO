// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by
// ${ACE_ROOT}/GenExportH.BAT
// ------------------------------
#if !defined (TCF_EXPORT_H)
#define TCF_EXPORT_H

#include "ace/OS.h"

#if !defined (TCF_HAS_DLL)
#define TCF_HAS_DLL 1
#endif /* ! TCF_HAS_DLL */

#if defined (TCF_HAS_DLL)
#  if (TCF_HAS_DLL == 1)
#    if defined (TCF_BUILD_DLL)
#      define TCF_Export ACE_Proper_Export_Flag
#      define TCF_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    else
#      define TCF_Export ACE_Proper_Import_Flag
#      define TCF_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    endif /* TCF_BUILD_DLL */
#  else
#    define TCF_Export
#    define TCF_SINGLETON_DECLARATION(T)
#  endif   /* ! TCF_HAS_DLL == 1 */
#else
#  define TCF_Export
#  define TCF_SINGLETON_DECLARATION(T)
#endif     /* TCF_HAS_DLL */

#endif     /* TCF_EXPORT_H */

// End of auto generated file.
